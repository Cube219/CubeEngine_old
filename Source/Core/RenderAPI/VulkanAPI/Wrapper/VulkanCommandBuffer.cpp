#include "VulkanCommandBuffer.h"

#include "EngineCore/Assertion.h"
#include "VulkanDevice.h"
#include "VulkanQueue.h"
#include "VulkanBuffer.h"
#include "VulkanGraphicsPipeline.h"
#include "VulkanRenderPass.h"
#include "VulkanFramebuffer.h"
#include "vulkanDescriptor.h"
#include "VulkanSemaphore.h"
#include "VulkanFence.h"
#include "VulkanGraphicsPipeline.h"
#include "VulkanImage.h"

namespace cube
{
	namespace render
	{
		VulkanCommandBuffer::VulkanCommandBuffer(SPtr<VulkanDevice>& device, SPtr<VulkanCommandPool>& commandPool, VkCommandBuffer commandBuffer) :
			mDevice_ref(device), mCommandPool_ref(commandPool),
			mCommandBuffer(commandBuffer), mIsRenderPassStarted(false)
		{
		}

		VulkanCommandBuffer::~VulkanCommandBuffer()
		{
			vkFreeCommandBuffers(mDevice_ref->GetHandle(), mCommandPool_ref->GetHandle(), 1, &mCommandBuffer);
		}

		void VulkanCommandBuffer::Reset()
		{
			VkResult res;

			res = vkResetCommandBuffer(mCommandBuffer, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);
			CheckVkResult("Cannot reset the command buffer", res);

			mIsRenderPassStarted = false;
		}

		void VulkanCommandBuffer::Begin()
		{
			// If the command buffer is a secondary, Begin will be defferd to SetRenderpass
			// because of pInheritanceInfo
			if(mIsPrimary == false)
				return;

			VkResult res;

			VkCommandBufferBeginInfo commandBufferBeginInfo = {};
			commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			commandBufferBeginInfo.pNext = nullptr;
			commandBufferBeginInfo.flags = 0;
			commandBufferBeginInfo.pInheritanceInfo = nullptr;

			res = vkBeginCommandBuffer(mCommandBuffer, &commandBufferBeginInfo);
			CheckVkResult("Cannot begin the command buffer (Primary)", res);
		}

		void VulkanCommandBuffer::CopyBuffer(SPtr<Buffer>& source, uint64_t sourceOffset,
			SPtr<Buffer>& destination, uint64_t destinationOffset, uint64_t size)
		{
			VkBufferCopy copy;
			copy.srcOffset = sourceOffset;
			copy.dstOffset = destinationOffset;
			copy.size = size;

			vkCmdCopyBuffer(mCommandBuffer, DPCast(VulkanBuffer)(source)->GetHandle(), DPCast(VulkanBuffer)(destination)->GetHandle(), 1, &copy);
		}

		void VulkanCommandBuffer::CopyBufferToImage(SPtr<Buffer>& buffer, uint64_t bufferOffset,
			SPtr<Image>& image, int imageOffsetX, int imageOffsetY, int imageOffsetZ,
			uint32_t imageWidth, uint32_t imageHeight, uint32_t imageDepth, ImageAspectBits aspectBits)
		{
			VkBufferImageCopy region;
			region.bufferOffset = bufferOffset;
			region.bufferRowLength = 0;
			region.bufferImageHeight = 0;

			region.imageSubresource.aspectMask = GetVkImageAspectFlags(aspectBits);
			region.imageSubresource.mipLevel = 0; // TODO: 차후 구현
			region.imageSubresource.baseArrayLayer = 0;
			region.imageSubresource.layerCount = 1;

			region.imageOffset = {imageOffsetX, imageOffsetY, imageOffsetZ};
			region.imageExtent = {imageWidth, imageHeight, imageDepth};

			vkCmdCopyBufferToImage(mCommandBuffer, DPCast(VulkanBuffer)(buffer)->GetHandle(), DPCast(VulkanImage)(image)->GetHandle(),
				VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
		}

		void VulkanCommandBuffer::SetRenderPass(SPtr<RenderPass>& renderPass, Rect2D renderArea)
		{
			auto vkRenderPass = DPCast(VulkanRenderPass)(renderPass);

			// If the command buffer is a secondary, Begin will be defferd to SetRenderpass
			// because of pInheritanceInfo
			if(mIsPrimary == false) {
				VkResult res;

				VkCommandBufferInheritanceInfo inheritInfo;
				inheritInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
				inheritInfo.pNext = nullptr;
				inheritInfo.renderPass = vkRenderPass->GetHandle();
				inheritInfo.subpass = 0;
				inheritInfo.framebuffer = vkRenderPass->GetFramebuffer()->GetHandle();
				inheritInfo.occlusionQueryEnable = VK_FALSE;
				inheritInfo.queryFlags = 0;
				inheritInfo.pipelineStatistics = 0;

				VkCommandBufferBeginInfo commandBufferBeginInfo = {};
				commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
				commandBufferBeginInfo.pNext = nullptr;
				commandBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
				commandBufferBeginInfo.pInheritanceInfo = &inheritInfo;

				res = vkBeginCommandBuffer(mCommandBuffer, &commandBufferBeginInfo);
				CheckVkResult("Cannot begin the command buffer (Secondary)", res);

				return;
			}

			if(mIsRenderPassStarted == true)
				vkCmdEndRenderPass(mCommandBuffer);

			VkRenderPassBeginInfo info;
			info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			info.pNext = nullptr;
			info.renderPass = vkRenderPass->GetHandle();
			info.framebuffer = vkRenderPass->GetFramebuffer()->GetHandle();
			info.renderArea = GetVkRect2D(renderArea);
			info.clearValueCount = SCast(uint32_t)(vkRenderPass->mAttachmentClearValues.size());
			info.pClearValues = vkRenderPass->mAttachmentClearValues.data();

			vkCmdBeginRenderPass(mCommandBuffer, &info, VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS); // TODO: Secondary command buffer인경우 처리 (VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS)

			mIsRenderPassStarted = true;
		}

		void VulkanCommandBuffer::PipelineBufferMemoryBarrier(PipelineStageBits srcStage, PipelineStageBits dstStage,
			AccessBits srcAccess, AccessBits dstAccess, SPtr<Buffer>& buffer, uint64_t offset, uint64_t size)
		{
			VkBufferMemoryBarrier barrier;
			barrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
			barrier.pNext = nullptr;
			barrier.srcAccessMask = GetVkAccessFlags(srcAccess);
			barrier.dstAccessMask = GetVkAccessFlags(dstAccess);
			barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barrier.buffer = SPCast(VulkanBuffer)(buffer)->GetHandle();
			barrier.offset = offset;
			barrier.size = size;

			vkCmdPipelineBarrier(mCommandBuffer, GetVkPipelineStageFlags(srcStage), GetVkPipelineStageFlags(dstStage), 0, 0, nullptr, 1, &barrier, 0, nullptr);
		}

		void VulkanCommandBuffer::PipelineImageMemoryBarrier(PipelineStageBits srcStage, PipelineStageBits dstStage,
			AccessBits srcAccess, AccessBits dstAccess, ImageLayout oldLayout, ImageLayout newLayout,
			SPtr<Image>& image)
		{
			VkImageMemoryBarrier barrier;
			barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			barrier.pNext = nullptr;
			barrier.srcAccessMask = GetVkAccessFlags(srcAccess);
			barrier.dstAccessMask = GetVkAccessFlags(dstAccess);
			barrier.oldLayout = GetVkImageLayout(oldLayout);
			barrier.newLayout = GetVkImageLayout(newLayout);
			barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barrier.image = DPCast(VulkanImage)(image)->GetHandle();

			barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT; // TODO: 차후 수정
			barrier.subresourceRange.baseMipLevel = 0;
			barrier.subresourceRange.levelCount = 1;
			barrier.subresourceRange.baseArrayLayer = 0;
			barrier.subresourceRange.layerCount = 1;

			vkCmdPipelineBarrier(mCommandBuffer, GetVkPipelineStageFlags(srcStage), GetVkPipelineStageFlags(dstStage), 0, 0, nullptr, 0, nullptr, 1, &barrier);
		}

		void VulkanCommandBuffer::SetViewport(uint32_t firstViewport, uint32_t viewportCount, Viewport* pViewports)
		{
			VkViewport* vps = new VkViewport[viewportCount];
			for(uint32_t i = 0; i < viewportCount; i++) {
				vps[i].x = pViewports[i].x;
				vps[i].y = pViewports[i].y;
				vps[i].width = pViewports[i].width;
				vps[i].height = pViewports[i].height;
				vps[i].minDepth = pViewports[i].minDepth;
				vps[i].maxDepth = pViewports[i].maxDepth;
			}

			vkCmdSetViewport(mCommandBuffer, firstViewport, viewportCount, vps);

			delete[] vps;
		}

		void VulkanCommandBuffer::SetScissor(uint32_t firstScissor, uint32_t scissorCount, Rect2D* pScissors)
		{
			VkRect2D* scs = new VkRect2D[scissorCount];
			for(uint32_t i = 0; i < scissorCount; i++) {
				scs[i].offset.x = pScissors[i].x;
				scs[i].offset.y = pScissors[i].y;
				scs[i].extent.width = pScissors[i].width;
				scs[i].extent.height = pScissors[i].height;
			}

			vkCmdSetScissor(mCommandBuffer, firstScissor, scissorCount, scs);

			delete[] scs;
		}

		void VulkanCommandBuffer::BindGraphicsPipeline(SPtr<GraphicsPipeline>& graphicsPipeline)
		{
			mGraphicsPipeline = DPCast(VulkanGraphicsPipeline)(graphicsPipeline);

			vkCmdBindPipeline(mCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, mGraphicsPipeline->GetHandle());
		}

		void VulkanCommandBuffer::BindDescriptorSets(PipelineType pipelineType, uint32_t firstSet,
			uint32_t descriptorSetNum, SPtr<DescriptorSet>* descriptorSets)
		{
			VkDescriptorSet* sets = new VkDescriptorSet[descriptorSetNum];
			for(uint32_t i = 0; i < descriptorSetNum; i++) {
				sets[i] = SPCast(VulkanDescriptorSet)(descriptorSets[i])->GetHandle();
			}

			vkCmdBindDescriptorSets(mCommandBuffer, GetVkPipelineBindPoint(pipelineType), mGraphicsPipeline->GetLayout(), firstSet, descriptorSetNum, sets, 0, nullptr);

			delete[] sets;
		}

		void VulkanCommandBuffer::BindVertexBuffers(uint32_t bufferNum, SPtr<Buffer>* buffers, uint64_t* bufferOffsets)
		{
			VkBuffer* buf = new VkBuffer[bufferNum];
			for(uint32_t i = 0; i < bufferNum; i++) {
				buf[i] = SPCast(VulkanBuffer)(buffers[i])->GetHandle();
			}

			vkCmdBindVertexBuffers(mCommandBuffer, 0, bufferNum, buf, bufferOffsets);

			delete[] buf;
		}

		void VulkanCommandBuffer::BindIndexBuffer(SPtr<Buffer> buffer, uint64_t bufferOffset)
		{
			vkCmdBindIndexBuffer(mCommandBuffer, SPCast(VulkanBuffer)(buffer)->GetHandle(), bufferOffset, VK_INDEX_TYPE_UINT32);
		}

		void VulkanCommandBuffer::Draw(uint32_t vertexCount, uint32_t vertexOffset, uint32_t instanceCount, uint32_t firstInstance)
		{
			vkCmdDraw(mCommandBuffer, vertexCount, instanceCount, vertexOffset, firstInstance);
		}

		void VulkanCommandBuffer::DrawIndexed(uint32_t indexCount, uint32_t indexOffset, int32_t vertexOffset, uint32_t instanceCount, uint32_t firstInstance)
		{
			vkCmdDrawIndexed(mCommandBuffer, indexCount, instanceCount, indexOffset, vertexOffset, firstInstance);
		}

		void VulkanCommandBuffer::ExecuteCommands(uint32_t commandCount, SPtr<CommandBuffer>* commandBuffers)
		{
			VkCommandBuffer* cmds = new VkCommandBuffer[commandCount];
			for(uint32_t i = 0; i < commandCount; i++) {
				cmds[i] = SPCast(VulkanCommandBuffer)(commandBuffers[i])->GetHandle();
			}

			vkCmdExecuteCommands(mCommandBuffer, commandCount, cmds);

			delete[] cmds;
		}

		void VulkanCommandBuffer::End()
		{
			if(mIsRenderPassStarted == true)
				vkCmdEndRenderPass(mCommandBuffer);

			VkResult res;

			res = vkEndCommandBuffer(mCommandBuffer);
			CheckVkResult("Cannot end the command buffer", res);
		}

		void VulkanCommandBuffer::Submit(SPtr<Queue>& queue,
			uint32_t waitSemaphoreNum, std::pair<SPtr<Semaphore>, PipelineStageBits>* waitSemaphores,
			uint32_t signalSemaphoreNum, SPtr<Semaphore>* signalSemaphores, SPtr<Fence> waitFence)
		{
			VkResult res;

			Vector<VkSemaphore> wait;
			Vector<VkPipelineStageFlags> waitStage;
			Vector<VkSemaphore> signal;

			wait.resize(waitSemaphoreNum);
			waitStage.resize(waitSemaphoreNum);
			for(uint32_t i = 0; i < waitSemaphoreNum; i++) {
				wait[i] = DPCast(VulkanSemaphore)(waitSemaphores[i].first)->GetHandle();
				waitStage[i] = GetVkPipelineStageFlags(waitSemaphores[i].second);
			}
			signal.resize(signalSemaphoreNum);
			for(uint32_t i = 0; i < signalSemaphoreNum; i++) {
				signal[i] = DPCast(VulkanSemaphore)(signalSemaphores[i])->GetHandle();
			}

			VkSubmitInfo submitInfo = {};

			submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			submitInfo.pNext = nullptr;
			submitInfo.waitSemaphoreCount = waitSemaphoreNum;
			submitInfo.pWaitSemaphores = wait.data();
			submitInfo.pWaitDstStageMask = waitStage.data();
			submitInfo.commandBufferCount = 1;
			submitInfo.pCommandBuffers = &mCommandBuffer;
			submitInfo.signalSemaphoreCount = signalSemaphoreNum;
			submitInfo.pSignalSemaphores = signal.data();

			VkFence f = NULL;
			if(waitFence != nullptr)
				f = DPCast(VulkanFence)(waitFence)->GetHandle();

			res = vkQueueSubmit(DPCast(VulkanQueue)(queue)->GetHandle(), 1, &submitInfo, f);
			CheckVkResult("Cannot submit the command buffer", res);
		}

		VkPipelineBindPoint VulkanCommandBuffer::GetVkPipelineBindPoint(PipelineType pipelineType)
		{
			VkPipelineBindPoint p;

			switch(pipelineType) {
				case PipelineType::Graphics:
					p = VK_PIPELINE_BIND_POINT_GRAPHICS;
					break;
				case PipelineType::Compute:
					p = VK_PIPELINE_BIND_POINT_COMPUTE;
					break;

				default:
					ASSERTION_FAILED("Unknown PipelineType ({0})", (int)pipelineType);
					break;
			}

			return p;
		}


		// -----------------------------------------
		//            VulkanCommandPool
		// -----------------------------------------

		VulkanCommandPool::VulkanCommandPool(SPtr<VulkanDevice>& device, VulkanQueueFamily designatedQueueFamily) :
			mDevice_ref(device), mDesignatedQueueFamily(designatedQueueFamily)
		{
			VkCommandPoolCreateInfo info = {};
			info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
			info.pNext = nullptr;
			info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
			info.queueFamilyIndex = designatedQueueFamily.mIndex;

			VkResult res;
			res = vkCreateCommandPool(device->GetHandle(), &info, nullptr, &mCommandPool);
			CheckVkResult("Cannot create VulkanCommandPool", res);
		}

		VulkanCommandPool::~VulkanCommandPool()
		{
			vkDestroyCommandPool(mDevice_ref->GetHandle(), mCommandPool, nullptr);
		}

		SPtr<VulkanCommandBuffer> VulkanCommandPool::AllocateCommandBuffer(VkCommandBufferLevel level)
		{
			VkResult res;

			VkCommandBuffer cmd;

			// Allocate command buffer
			VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
			commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			commandBufferAllocateInfo.pNext = nullptr;
			commandBufferAllocateInfo.commandPool = mCommandPool;
			commandBufferAllocateInfo.level = level;
			commandBufferAllocateInfo.commandBufferCount = 1;
			res = vkAllocateCommandBuffers(mDevice_ref->GetHandle(), &commandBufferAllocateInfo, &cmd);
			CheckVkResult("Cannot allocate VulkanCommandBuffer", res);

			SPtr<VulkanCommandBuffer> cmdBuffer(new VulkanCommandBuffer(mDevice_ref, shared_from_this(), cmd));

			if(level == VK_COMMAND_BUFFER_LEVEL_PRIMARY)
				cmdBuffer->mIsPrimary = true;
			else
				cmdBuffer->mIsPrimary = false;

			return cmdBuffer;
		}
	} // namespace render
} // namespace cube
