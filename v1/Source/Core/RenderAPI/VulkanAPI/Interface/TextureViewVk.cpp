#include "TextureViewVk.h"

#include "DeviceVk.h"
#include "TextureVk.h"
#include "../VulkanTypeConversion.h"
#include "EngineCore/Assertion.h"

namespace cube
{
	namespace render
	{
		TextureViewVk::TextureViewVk(DeviceVk& device, const TextureViewAttribute& attr, const TextureVk& image) :
			TextureView(attr),
			mParentImage(image)
		{
			VkImageViewCreateInfo info = {};
			info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			info.pNext = nullptr;
			info.flags = 0;
			info.image = image.GetHandle();

			switch(attr.type) {
				case TextureViewType::Texture1D:        info.viewType = VK_IMAGE_VIEW_TYPE_1D; break;
				case TextureViewType::Texture1DArray:   info.viewType = VK_IMAGE_VIEW_TYPE_1D_ARRAY; break;
				case TextureViewType::Texture2D:        info.viewType = VK_IMAGE_VIEW_TYPE_2D; break;
				case TextureViewType::Texture2DArray:   info.viewType = VK_IMAGE_VIEW_TYPE_2D_ARRAY; break;
				case TextureViewType::Texture3D:        info.viewType = VK_IMAGE_VIEW_TYPE_3D; break;
				case TextureViewType::TextureCube:      info.viewType = VK_IMAGE_VIEW_TYPE_CUBE; break;
				case TextureViewType::TextureCubeArray: info.viewType = VK_IMAGE_VIEW_TYPE_CUBE_ARRAY; break;
				default: ASSERTION_FAILED("Unknown TextureViewType ({0})", (Uint32)attr.type);  break;
			}
			
			info.format = TextureFormatToVkFormat(attr.format);
			mFormat = info.format;
			info.components = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
			info.subresourceRange.baseMipLevel = attr.firstMipLevel;
			info.subresourceRange.levelCount = attr.numMipLevels;

			if(attr.type == TextureViewType::Texture1DArray ||
				attr.type == TextureViewType::Texture2DArray ||
				attr.type == TextureViewType::TextureCubeArray) {
				info.subresourceRange.baseArrayLayer = attr.firstArrayIndex;
				info.subresourceRange.layerCount = attr.numArray;
			} else {
				info.subresourceRange.baseArrayLayer = 0;
				info.subresourceRange.layerCount = 1;
			}

			info.subresourceRange.aspectMask = 0;
			if(attr.componentFlags.IsSet(TextureViewComponentFlag::Color)) {
				info.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_COLOR_BIT;
			}
			if(attr.componentFlags.IsSet(TextureViewComponentFlag::Depth)) {
				info.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_DEPTH_BIT;
			}
			if(attr.componentFlags.IsSet(TextureViewComponentFlag::Stencil)) {
				info.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
			}

			U8String debugName = fmt::format("{0} (Texture: {1})", attr.debugName, image.mImage.mDebugName);
			mImageView = device.GetLogicalDevice()->CreateVkImageViewWrapper(info, debugName.c_str());
		}

		TextureViewVk::~TextureViewVk()
		{
			mImageView.Release();
		}
	} // namespace render
} // namespace cube
