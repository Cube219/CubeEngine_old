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
			Texture(attr),
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
			info.extent = { attr.width, attr.height, attr.depth };
			info.mipLevels = attr.mipLevels;
			info.arrayLayers = attr.arraySize;
			info.samples = VK_SAMPLE_COUNT_1_BIT; // TODO: 차후 구현
			info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			mCurrentLayout = VK_IMAGE_LAYOUT_UNDEFINED;

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

				CommandListAttribute cmdListAttr;
				cmdListAttr.usage = CommandListUsage::TransferImmediate;
				cmdListAttr.threadIndex = 0;
				cmdListAttr.isSub = false;
				cmdListAttr.debugName = "Command List to transfer texture data";
				SPtr<CommandListVk> cmdBuf = cmdListPool.Allocate(cmdListAttr);
				cmdBuf->Begin();
				VkCommandBuffer vkCmdBuf = cmdBuf->GetHandle();

				TransitionImageLayout(*cmdBuf, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

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
				bufImgCopy.imageSubresource.layerCount = attr.arraySize;

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

		void TextureVk::CopyFromTexture(CommandList& cmdList, Texture& src, Uint32 srcMipLevel, Uint32 srcArrayIndex, const Rect3D* pSrcArea,
			Uint32 dstMipLevel, Uint32 dstArrayIndex, Uint32 dstX, Uint32 dstY, Uint32 dstZ)
		{
			VkImageCopy copy;

			if(pSrcArea == nullptr) {
				copy.srcOffset.x = 0;
				copy.srcOffset.y = 0;
				copy.srcOffset.z = 0;
				copy.extent.width = mWidth >> srcMipLevel;
				copy.extent.height = mHeight >> srcMipLevel;
				if(mType == TextureType::Texture3D)
					copy.extent.depth = mDepth >> srcMipLevel;
				else
					copy.extent.depth = 1;
			} else {
				copy.srcOffset.x = pSrcArea->x;
				copy.srcOffset.y = pSrcArea->y;
				copy.srcOffset.z = pSrcArea->z;
				copy.extent.width = pSrcArea->width;
				copy.extent.height = pSrcArea->height;
				copy.extent.depth = pSrcArea->depth;
			}

			VkImageAspectFlags aspectMask = 0;
			if(mBindTypeFlags.IsSet(TextureBindTypeFlag::DepthStencil)) {
				aspectMask |= VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
			} else {
				aspectMask |= VK_IMAGE_ASPECT_COLOR_BIT;
			}

			copy.srcSubresource.aspectMask = aspectMask;
			copy.srcSubresource.mipLevel = srcMipLevel;
			copy.srcSubresource.baseArrayLayer = srcArrayIndex;
			copy.srcSubresource.layerCount = 1;

			copy.dstSubresource.aspectMask = aspectMask;
			copy.dstSubresource.mipLevel = dstMipLevel;
			copy.dstSubresource.baseArrayLayer = dstArrayIndex;
			copy.dstSubresource.layerCount = 1;

			copy.dstOffset.x = dstX;
			copy.dstOffset.y = dstY;
			copy.dstOffset.z = dstZ;

			TextureVk& srcVk = DCast(TextureVk&)(src);
			CommandListVk& cmdListVk = DCast(CommandListVk&)(cmdList);

			if(srcVk.GetCurrentLayout() != VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL)
				srcVk.TransitionImageLayout(cmdListVk, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
			if(mCurrentLayout != VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
				TransitionImageLayout(cmdListVk, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

			vkCmdCopyImage(cmdListVk.GetHandle(), srcVk.GetHandle(), VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, mImage.mObject, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				1, &copy);
		}

		SPtr<TextureView> TextureVk::CreateView(const TextureViewAttribute& attr)
		{
			return std::make_shared<TextureViewVk>(mDevice, attr, *this);
		}

		void TextureVk::TransitionImageLayout(CommandListVk& cmdList, VkImageLayout newLayout)
		{
			CHECK(mCurrentLayout != newLayout, "Already transitioned new layout({0}).", (int)newLayout);

			VkCommandBuffer vkCmdBuf = cmdList.GetHandle();

			VkImageMemoryBarrier imgBarrier;
			imgBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			imgBarrier.pNext = nullptr;
			imgBarrier.srcAccessMask = GetAccessMask(mCurrentLayout);
			imgBarrier.dstAccessMask = GetAccessMask(newLayout);
			imgBarrier.oldLayout = mCurrentLayout;
			imgBarrier.newLayout = newLayout;
			imgBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			imgBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			imgBarrier.image = mImage.mObject;
			imgBarrier.subresourceRange.aspectMask = 0;
			imgBarrier.subresourceRange.baseMipLevel = 0;
			imgBarrier.subresourceRange.levelCount = VK_REMAINING_MIP_LEVELS;
			imgBarrier.subresourceRange.baseArrayLayer = 0;
			imgBarrier.subresourceRange.layerCount = VK_REMAINING_ARRAY_LAYERS;

			if(mBindTypeFlags.IsSet(TextureBindTypeFlag::DepthStencil)) {
				imgBarrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
			} else {
				imgBarrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_COLOR_BIT;
			}

			vkCmdPipelineBarrier(vkCmdBuf, GetPipelineStageFlags(mCurrentLayout), GetPipelineStageFlags(newLayout), 0,
				0, nullptr, 0, nullptr, 1, &imgBarrier);

			mCurrentLayout = newLayout;
		}

		VkAccessFlags TextureVk::GetAccessMask(VkImageLayout layout)
		{
			switch(layout) {
				case VK_IMAGE_LAYOUT_UNDEFINED:
					return 0;

				case VK_IMAGE_LAYOUT_GENERAL:
					return VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_SHADER_WRITE_BIT;

				case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
					return VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

				case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
					return VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

				case VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL:
					return VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;

				case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
					return VK_ACCESS_SHADER_READ_BIT;

				case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
					return VK_ACCESS_TRANSFER_READ_BIT;

				case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
					return VK_ACCESS_TRANSFER_WRITE_BIT;

				case VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL:
				case VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL:
					return VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;

				case VK_IMAGE_LAYOUT_PRESENT_SRC_KHR:
					return VK_ACCESS_MEMORY_READ_BIT;

				case VK_IMAGE_LAYOUT_SHARED_PRESENT_KHR:
					return VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT;

				default:
					ASSERTION_FAILED("Unknown VkImageLayout({0}).", (int)layout);
					return 0;
			}
		}

		VkPipelineStageFlags TextureVk::GetPipelineStageFlags(VkImageLayout layout)
		{
			switch(layout) {
				case VK_IMAGE_LAYOUT_UNDEFINED:
				case VK_IMAGE_LAYOUT_GENERAL:
					return VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;

				case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
					return VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

				case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
					return VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;

				case VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL:
				case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
					return VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;

				case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
				case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
					return VK_PIPELINE_STAGE_TRANSFER_BIT;

				case VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL:
				case VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL:
					return VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;

				case VK_IMAGE_LAYOUT_PRESENT_SRC_KHR:
					return VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;

				case VK_IMAGE_LAYOUT_SHARED_PRESENT_KHR:
					return VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;

				default:
					ASSERTION_FAILED("Unknown VkImageLayout({0}).", (int)layout);
					return 0;
			}
		}
	} // namespace render
} // namespace cube
