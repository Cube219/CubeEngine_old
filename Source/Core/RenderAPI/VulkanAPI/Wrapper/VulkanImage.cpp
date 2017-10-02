#include "VulkanImage.h"

#include "VulkanDevice.h"

namespace cube
{
	namespace core
	{
		VkImageType GetVkImageType(ImageType type)
		{
			VkImageType t;

			switch(type) {
				case ImageType::Image1D:
					t = VK_IMAGE_TYPE_1D;
					break;
				case ImageType::Image2D:
					t = VK_IMAGE_TYPE_2D;
					break;
				case ImageType::Image3D:
					t = VK_IMAGE_TYPE_3D;
					break;

				default:
					PrintlnLogWithSayer(L"VulkanImage", L"Unknown ImageType");
					break;
			}
			return t;
		}

		VkImageUsageFlags GetVkImageUsageFlags(ImageUsageBits usage)
		{
			VkImageUsageFlags f = 0;

			if(static_cast<int>(usage & ImageUsageBits::TransferSourceBit) > 0)
				f |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;

			if(static_cast<int>(usage & ImageUsageBits::TransferDestinationBit) > 0)
				f |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;

			if(static_cast<int>(usage & ImageUsageBits::SampledBit) > 0)
				f |= VK_IMAGE_USAGE_SAMPLED_BIT;

			if(static_cast<int>(usage & ImageUsageBits::StorageBit) > 0)
				f |= VK_IMAGE_USAGE_STORAGE_BIT;

			if(static_cast<int>(usage & ImageUsageBits::ColorAttachmentBit) > 0)
				f |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

			if(static_cast<int>(usage & ImageUsageBits::DepthStencilAttachmentBit) > 0)
				f |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;

			if(static_cast<int>(usage & ImageUsageBits::TransientAttachmentBit) > 0)
				f |= VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT;

			if(static_cast<int>(usage & ImageUsageBits::InputAttachmentBit) > 0)
				f |= VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT;

			return f;
		}

		VkImageViewType GetVkImageViewType(ImageViewType type)
		{
			VkImageViewType t;

			switch(type) {
				case ImageViewType::Image1D:
					t = VK_IMAGE_VIEW_TYPE_1D;
					break;
				case ImageViewType::Image2D:
					t = VK_IMAGE_VIEW_TYPE_2D;
					break;
				case ImageViewType::Image3D:
					t = VK_IMAGE_VIEW_TYPE_3D;
					break;
				case ImageViewType::Cube:
					t = VK_IMAGE_VIEW_TYPE_CUBE;
					break;
				case ImageViewType::Image1DArray:
					t = VK_IMAGE_VIEW_TYPE_1D_ARRAY;
					break;
				case ImageViewType::Image2DArray:
					t = VK_IMAGE_VIEW_TYPE_2D_ARRAY;
					break;
				case ImageViewType::CubeArray:
					t = VK_IMAGE_VIEW_TYPE_CUBE_ARRAY;
					break;

				default:
					PrintlnLogWithSayer(L"VulkanImage", L"Unknown ImageViewType");
					break;
			}

			return t;
		}

		VkImageAspectFlags GetVkImageAspectFlags(ImageAspectBits aspect)
		{
			VkImageAspectFlags a = 0;

			if(static_cast<int>(aspect & ImageAspectBits::Color) > 0)
				a |= VK_IMAGE_ASPECT_COLOR_BIT;

			if(static_cast<int>(aspect & ImageAspectBits::Depth) > 0)
				a |= VK_IMAGE_ASPECT_DEPTH_BIT;

			if(static_cast<int>(aspect & ImageAspectBits::Stencil) > 0)
				a |= VK_IMAGE_ASPECT_STENCIL_BIT;

			if(static_cast<int>(aspect & ImageAspectBits::Metadata) > 0)
				a |= VK_IMAGE_ASPECT_METADATA_BIT;

			return a;
		}

		VkImageLayout GetVkImageLayout(ImageLayout imageLayout)
		{
			VkImageLayout l;

			switch(imageLayout) {
				case ImageLayout::Undefined:
					l = VK_IMAGE_LAYOUT_UNDEFINED;
					break;
				case ImageLayout::General:
					l = VK_IMAGE_LAYOUT_GENERAL;
					break;
				case ImageLayout::ColorAttachmentOptimal:
					l = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
					break;
				case ImageLayout::DepthStencilAttachmentOptimal:
					l = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
					break;
				case ImageLayout::DepthStencilReadOnlyOptimal:
					l = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
					break;
				case ImageLayout::ShaderReadOnlyOptimal:
					l = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
					break;
				case ImageLayout::TransferSourceOptimal:
					l = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
					break;
				case ImageLayout::TransferDestinationOptimal:
					l = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
					break;
				case ImageLayout::Preinitialized:
					l = VK_IMAGE_LAYOUT_PREINITIALIZED;
					break;
				case ImageLayout::PresentSource:
					l = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
					break;
				case ImageLayout::SharedPresent:
					l = VK_IMAGE_LAYOUT_SHARED_PRESENT_KHR;
					break;

				default:
					PrintlnLogWithSayer(L"VulkanImage", L"Unknown ImageLayout");
					break;
			}

			return l;
		}

		VulkanImageView::VulkanImageView(const SPtr<VulkanDevice>& device, const SPtr<VulkanImage>& image,
			VkFormat format, VkComponentMapping components, VkImageSubresourceRange subresourceRange,
			VkImageViewType viewType) :
			mDevice_ref(device)
		{
			VkResult res;

			VkImageViewCreateInfo imageViewCreateinfo = {};
			imageViewCreateinfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			imageViewCreateinfo.pNext = nullptr;
			imageViewCreateinfo.flags = 0;
			imageViewCreateinfo.image = *image;
			imageViewCreateinfo.format = format;
			imageViewCreateinfo.components = components;
			imageViewCreateinfo.subresourceRange = subresourceRange;
			imageViewCreateinfo.viewType = viewType;

			res = vkCreateImageView(*mDevice_ref, &imageViewCreateinfo, nullptr, &mImageView);
			CheckVkResult(L"VulkanImageView", L"Cannot create a VulkanImageView", res);

			mFormat = format;
		}

		VulkanImageView::~VulkanImageView()
		{
			vkDestroyImageView(*mDevice_ref, mImageView, nullptr);
		}

		VkFormat VulkanImageView::GetVkFormat() const
		{
			return mFormat;
		}

		VulkanImage::VulkanImage(const SPtr<VulkanDevice>& device, VkImage image) :
			mDevice_ref(device), mAllocatedMemory(0)
		{
			mImage = image;
		}

		VulkanImage::VulkanImage(const SPtr<VulkanDevice>& device,
			VkImageType type, VkFormat format, VkExtent3D extent, uint32_t mipLevels, uint32_t arrayLayers,
			VkSampleCountFlagBits samples, VkImageLayout initialLayout, VkImageUsageFlags usage, VkSharingMode sharingMode, bool optimal) :
			mDevice_ref(device)
		{
			VkResult res;

			VkImageCreateInfo imageCreateInfo = {};
			imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
			imageCreateInfo.pNext = nullptr;
			imageCreateInfo.flags = 0;
			imageCreateInfo.imageType = type;
			imageCreateInfo.format = format;
			imageCreateInfo.extent = extent;
			imageCreateInfo.mipLevels = mipLevels;
			imageCreateInfo.arrayLayers = arrayLayers;
			imageCreateInfo.samples = samples;
			imageCreateInfo.initialLayout = initialLayout;
			imageCreateInfo.usage = usage;
			imageCreateInfo.queueFamilyIndexCount = 0;
			imageCreateInfo.pQueueFamilyIndices = nullptr;
			imageCreateInfo.sharingMode = sharingMode;

			if(optimal == true)
				imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
			else
				imageCreateInfo.tiling = VK_IMAGE_TILING_LINEAR;

			res = vkCreateImage(*device, &imageCreateInfo, nullptr, &mImage);
			CheckVkResult(L"VulkanImage", L"Cannot create VulkanImage", res);

			// Allocate memory and bind
			VkMemoryRequirements memRequire;
			vkGetImageMemoryRequirements(*device, mImage, &memRequire);

			mAllocatedMemory = device->AllocateMemory(memRequire, 0);
			res = vkBindImageMemory(*device, mImage, mAllocatedMemory, 0);
			CheckVkResult(L"VulkanImage", L"Cannot bind memory to the image", res);
		}

		VulkanImage::~VulkanImage()
		{
			// Destroy image only if it is created by itself
			if(mAllocatedMemory != NULL) {
				vkDestroyImage(*mDevice_ref, mImage, nullptr);
				vkFreeMemory(*mDevice_ref, mAllocatedMemory, nullptr);
			}
		}

		SPtr<BaseRenderImageView> VulkanImage::GetImageView(DataFormat format, ImageAspectBits aspectBits, ImageViewType type)
		{
			// TODO: ���� mapping ���� �߰�...?
			VkComponentMapping mapping = {VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A};
			// TODO: ���� �ؿ� ���� �߰�?? (mipmap / layer...)
			VkImageSubresourceRange range = {GetVkImageAspectFlags(aspectBits), 0, 1, 0, 1};

			return std::make_shared<VulkanImageView>(mDevice_ref, shared_from_this(),
				GetVkFormat(format), mapping, range, GetVkImageViewType(type));
		}
	}
}