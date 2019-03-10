#include "TextureVk.h"

#include "../VulkanUtility.h"
#include "../VulkanTypeConversion.h"
#include "DeviceVk.h"
#include "TextureViewVk.h"
#include "EngineCore/Assertion.h"

namespace cube
{
	namespace render
	{
		TextureVk::TextureVk(DeviceVk& device, const TextureAttribute& attr,
			VulkanQueueManager& queueManager, VulkanCommandListPool& cmdListPool) : 
			Texture(attr.debugName),
			mDevice(device)
		{
			VkResult res;

			VkImageCreateInfo info;
			info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
			info.pNext = nullptr;
			info.flags = 0;
			
			switch(attr.type) {
				case TextureType::Texture1D:
				case TextureType::Texture1DArray:
					info.imageType = VK_IMAGE_TYPE_1D;
					break;

				case TextureType::TextureCube:
				case TextureType::TextureCubeArray:
					info.flags |= VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
					// Fallback

				case TextureType::Texture2D:
				case TextureType::Texture2DArray:
					info.imageType = VK_IMAGE_TYPE_2D;
					break;

				case TextureType::Texture3D:
					info.imageType = VK_IMAGE_TYPE_3D;
					break;

				default:
					ASSERTION_FAILED("Unknown TextureType ({0})", (Uint32)attr.type);
					break;
			}

			info.format = TextureFormatToVkFormat(attr.format);
			info.extent = {attr.width, attr.height, attr.depth};
			info.mipLevels = attr.mipLevels;
			info.arrayLayers = 1; // TODO: 차후 구현
			info.samples = VK_SAMPLE_COUNT_1_BIT; // TODO: 차후 구현
			info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED; // 차후 구현?

			info.usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
			info.usage |= VK_IMAGE_USAGE_SAMPLED_BIT;

			if(attr.bindTypeFlags.IsSet(TextureBindTypeFlag::RenderTarget)) {
				info.usage |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
			}
			if(attr.bindTypeFlags.IsSet(TextureBindTypeFlag::ShaderResource)) {
				info.usage |= VK_IMAGE_USAGE_STORAGE_BIT;
			}
			if(attr.bindTypeFlags.IsSet(TextureBindTypeFlag::DepthStencil)) {
				info.usage |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
			}
			mUsage = info.usage;

			info.queueFamilyIndexCount = 0;
			info.pQueueFamilyIndices = nullptr;
			info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
			info.tiling = VK_IMAGE_TILING_OPTIMAL;

			mImage = device.GetLogicalDevice()->CreateVkImageWrapper(info, attr.debugName);

			// Allocate memory
			MemoryUsage memUsage;
			switch(attr.usage) {
			case ResourceUsage::Default:
			case ResourceUsage::Immutable:
				memUsage = MemoryUsage::GPU;
				break;

			case ResourceUsage::Dynamic: // TODO: 차후 구현
				ASSERTION_FAILED("Dynamic texture is not supported yet.");
				// memUsage = MemoryUsage::CPUtoGPU;
				break;

			case ResourceUsage::Staging: // TODO: 차후 구현
				ASSERTION_FAILED("Staging texture is not supported yet.");
				// memUsage = MemoryUsage::CPU;
				break;
			}

			if(attr.isDedicated) {
				VkImageMemoryRequirementsInfo2 memRequireInfo;
				memRequireInfo.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_REQUIREMENTS_INFO_2;
				memRequireInfo.pNext = nullptr;
				memRequireInfo.image = mImage.mObject;

				VkMemoryDedicatedRequirements memDediRequire;
				memDediRequire.sType = VK_STRUCTURE_TYPE_MEMORY_DEDICATED_REQUIREMENTS;
				memDediRequire.pNext = nullptr;
				memDediRequire.prefersDedicatedAllocation = VK_TRUE;
				memDediRequire.requiresDedicatedAllocation = VK_TRUE;

				VkMemoryRequirements2 memRequire;
				memRequire.sType = VK_STRUCTURE_TYPE_MEMORY_REQUIREMENTS_2;
				memRequire.pNext = &memDediRequire;

				vkGetImageMemoryRequirements2(mImage.GetVkDevice(), &memRequireInfo, &memRequire);

				VkMemoryDedicatedAllocateInfo dediAllocInfo;
				dediAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_DEDICATED_ALLOCATE_INFO;
				dediAllocInfo.pNext = nullptr;
				dediAllocInfo.buffer = VK_NULL_HANDLE;
				dediAllocInfo.image = mImage.mObject;

				mAllocation = device.GetMemoryManager().AllocateDedicated(memRequire, dediAllocInfo, memUsage);
			} else {
				VkMemoryRequirements memRequire;
				vkGetImageMemoryRequirements(mImage.GetVkDevice(), mImage.mObject, &memRequire);

				mAllocation = device.GetMemoryManager().Allocate(memRequire, memUsage);
			}

			// Bind memory
			res = vkBindImageMemory(mImage.GetVkDevice(), mImage.mObject, mAllocation.deviceMemory, mAllocation.offset);
			CHECK_VK(res, "Failed to bind buffer memory.");

			// Initialize texture data if it is existed
			if(attr.textureData.pData != nullptr && attr.textureData.size > 0) {
				VkBufferCreateInfo stagingBufInfo;
				stagingBufInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
				stagingBufInfo.pNext = nullptr;
				stagingBufInfo.flags = 0;
				stagingBufInfo.size = attr.textureData.size;
				stagingBufInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
				stagingBufInfo.queueFamilyIndexCount = 0;
				stagingBufInfo.pQueueFamilyIndices = nullptr;
				stagingBufInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

				U8String stagingBufDebugName = fmt::format("Staging buffer for '{0}'", attr.debugName);

				VkBufferWrapper stagingBuf =
					device.GetLogicalDevice()->CreateVkBufferWrapper(stagingBufInfo, stagingBufDebugName.c_str());

				VkMemoryRequirements stagingMemRequire;
				vkGetBufferMemoryRequirements(stagingBuf.GetVkDevice(), stagingBuf.mObject, &stagingMemRequire);
				VulkanAllocation stagingAlloc = device.GetMemoryManager().Allocate(stagingMemRequire, MemoryUsage::CPU);

				void* stagingData = stagingAlloc.mappedData;
				memcpy(stagingData, attr.textureData.pData, attr.textureData.size);

				res = vkBindBufferMemory(stagingBuf.GetVkDevice(), stagingBuf.mObject,
					stagingAlloc.pPage->GetVkDeviceMemory(), stagingAlloc.offset);
				CHECK_VK(res, "Failed to bind staging buffer memory.");

				SPtr<CommandListVk> cmdBuf = cmdListPool.Allocate(CommandListUsage::TransferImmediate, 0, false);
				cmdBuf->Begin();
				VkCommandBuffer vkCmdBuf = cmdBuf->GetHandle();

				VkImageMemoryBarrier imgBarrier;
				imgBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
				imgBarrier.pNext = nullptr;
				imgBarrier.srcAccessMask = 0; // NONE;
				imgBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
				imgBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				imgBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
				imgBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
				imgBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
				imgBarrier.image = mImage.mObject;
				imgBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT; // TODO: 차후 수정?
				imgBarrier.subresourceRange.baseMipLevel = 0;
				imgBarrier.subresourceRange.levelCount = 1;
				imgBarrier.subresourceRange.baseArrayLayer = 0;
				imgBarrier.subresourceRange.layerCount = 1;
				vkCmdPipelineBarrier(vkCmdBuf, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
					0, nullptr, 0, nullptr, 1, &imgBarrier);

				VkBufferImageCopy bufImgCopy;
				bufImgCopy.bufferOffset = 0;
				bufImgCopy.bufferRowLength = 0;
				bufImgCopy.bufferImageHeight = 0;

				bufImgCopy.imageSubresource.aspectMask = 0;
				if(attr.bindTypeFlags.IsSet(TextureBindTypeFlag::DepthStencil)) {
					bufImgCopy.imageSubresource.aspectMask |= VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
				} else {
					bufImgCopy.imageSubresource.aspectMask |= VK_IMAGE_ASPECT_COLOR_BIT;
				}
				bufImgCopy.imageSubresource.mipLevel = 0;
				bufImgCopy.imageSubresource.baseArrayLayer = 0;
				bufImgCopy.imageSubresource.layerCount = 1;

				bufImgCopy.imageOffset = { 0, 0, 0 };
				bufImgCopy.imageExtent = { attr.width, attr.height, attr.depth };

				vkCmdCopyBufferToImage(vkCmdBuf, stagingBuf.mObject, mImage.mObject, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &bufImgCopy);
				cmdBuf->End();

				queueManager.SubmitCommandList(*cmdBuf);

				device.ReleaseAtNextFrame(cmdBuf);
				device.ReleaseAtNextFrame(std::move(stagingAlloc));
				device.ReleaseAtNextFrame(std::move(stagingBuf));
			}

			// Setup default view attribute
			switch(attr.type) {
				case TextureType::Texture1D:        mDefaultViewAttr.type = TextureViewType::Texture1D; break;
				case TextureType::Texture1DArray:   mDefaultViewAttr.type = TextureViewType::Texture1DArray; break;
				case TextureType::Texture2D:        mDefaultViewAttr.type = TextureViewType::Texture2D; break;
				case TextureType::Texture2DArray:   mDefaultViewAttr.type = TextureViewType::Texture2DArray; break;
				case TextureType::Texture3D:        mDefaultViewAttr.type = TextureViewType::Texture3D; break;
				case TextureType::TextureCube:      mDefaultViewAttr.type = TextureViewType::TextureCube; break;
				case TextureType::TextureCubeArray: mDefaultViewAttr.type = TextureViewType::TextureCubeArray; break;
				default: break;
			}

			mDefaultViewAttr.format = attr.format;
			
			if(attr.bindTypeFlags.IsSet(TextureBindTypeFlag::DepthStencil)) {
				info.usage |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
				mDefaultViewAttr.componentFlags = TextureViewComponentFlag::Depth | TextureViewComponentFlag::Stencil;
			} else {
				mDefaultViewAttr.componentFlags = TextureViewComponentFlag::Color;
			}
			
			mDefaultViewAttr.firstMipLevel = 0;
			mDefaultViewAttr.numMipLevels = attr.mipLevels;
			mDefaultViewAttr.firstArrayIndex = 0;
			mDefaultViewAttr.numArray = attr.arraySize;
		}

		TextureVk::~TextureVk()
		{
			mAllocation.Free();
			mImage.Release();
		}

		void TextureVk::UpdateData(CommandList& cmdList, const void* pData, Uint64 size, Uint32 width, Uint32 height)
		{
			// TODO: 차후 구현
		}

		SPtr<TextureView> TextureVk::CreateView(const TextureViewAttribute& attr)
		{
			return std::make_shared<TextureViewVk>(mDevice, attr, *this);
		}
	} // namespace render
} // namespace cube
