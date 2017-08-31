#include "VulkanCommandBuffer.h"

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

namespace cube
{
	namespace core
	{
		VulkanCommandBuffer::VulkanCommandBuffer(SPtr<VulkanDevice>& device, SPtr<VulkanCommandPool>& commandPool, VkCommandBuffer commandBuffer) :
			mDevice_ref(device), mCommandPool_ref(commandPool),
			mCommandBuffer(commandBuffer), mIsRenderPassStarted(false)
		{
		}

		VulkanCommandBuffer::~VulkanCommandBuffer()
		{
			vkFreeCommandBuffers(*mDevice_ref, *mCommandPool_ref, 1, &mCommandBuffer);
		}

		void VulkanCommandBuffer::Reset()
		{
			VkResult res;

			res = vkResetCommandBuffer(mCommandBuffer, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);
			CheckVkResult(L"VulkanCommandBuffer", L"Cannot reset the command buffer", res);

			mIsRenderPassStarted = false;
		}

		void VulkanCommandBuffer::Begin()
		{
			VkResult res;

			VkCommandBufferBeginInfo commandBufferBeginInfo = {};
			commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			commandBufferBeginInfo.pNext = nullptr;
			commandBufferBeginInfo.flags = 0;
			commandBufferBeginInfo.pInheritanceInfo = nullptr;

			res = vkBeginCommandBuffer(mCommandBuffer, &commandBufferBeginInfo);
			CheckVkResult(L"VulkanCommandBuffer", L"Cannot begin the command buffer", res);
		}

		void VulkanCommandBuffer::SetRenderPass(SPtr<BaseRenderRenderPass>& renderPass, Rect2D renderArea)
		{
			if(mIsRenderPassStarted == true)
				vkCmdEndRenderPass(mCommandBuffer);

			auto vkRenderPass = DPCast(VulkanRenderPass)(renderPass);

			VkRenderPassBeginInfo info;
			info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			info.pNext = nullptr;
			info.renderPass = *vkRenderPass;
			info.framebuffer = *(vkRenderPass->GetFramebuffer());
			info.renderArea = GetVkRect2D(renderArea);
			info.clearValueCount = SCast(uint32_t)(vkRenderPass->mAttachmentClearValues.size());
			info.pClearValues = vkRenderPass->mAttachmentClearValues.data();

			vkCmdBeginRenderPass(mCommandBuffer, &info, VK_SUBPASS_CONTENTS_INLINE); // TODO: Secondary command buffer인경우 처리 (VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS)

			mIsRenderPassStarted = true;
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

		void VulkanCommandBuffer::BindGraphicsPipeline(SPtr<BaseRenderGraphicsPipeline>& graphicsPipeline)
		{
			mGraphicsPipeline = DPCast(VulkanGraphicsPipeline)(graphicsPipeline);

			vkCmdBindPipeline(mCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, *mGraphicsPipeline);
		}

		void VulkanCommandBuffer::BindDescriptorSets(PipelineType pipelineType, uint32_t firstSet,
			uint32_t descriptorSetNum, SPtr<BaseRenderDescriptorSet>* descriptorSets)
		{
			VkDescriptorSet* sets = new VkDescriptorSet[descriptorSetNum];
			for(uint32_t i = 0; i < descriptorSetNum; i++) {
				sets[i] = *SPCast(VulkanDescriptorSet)(descriptorSets[i]);
			}

			vkCmdBindDescriptorSets(mCommandBuffer, GetVkPipelineBindPoint(pipelineType), mGraphicsPipeline->GetLayout(), firstSet, descriptorSetNum, sets, 0, nullptr);

			delete[] sets;
		}

		void VulkanCommandBuffer::BindVertexBuffers(uint32_t bufferNum, SPtr<BaseRenderBuffer>* buffers, uint64_t* bufferOffsets)
		{
			VkBuffer* buf = new VkBuffer[bufferNum];
			for(uint32_t i = 0; i < bufferNum; i++) {
				buf[i] = *SPCast(VulkanBuffer)(buffers[i]);
			}

			vkCmdBindVertexBuffers(mCommandBuffer, 1, bufferNum, buf, bufferOffsets);

			delete[] buf;
		}

		void VulkanCommandBuffer::BindIndexBuffer(SPtr<BaseRenderBuffer> buffer, uint64_t bufferOffset)
		{
			vkCmdBindIndexBuffer(mCommandBuffer, *SPCast(VulkanBuffer)(buffer), bufferOffset, VK_INDEX_TYPE_UINT32);
		}

		void VulkanCommandBuffer::Draw(uint32_t vertexCount, uint32_t vertexOffset, uint32_t instanceCount, uint32_t firstInstance)
		{
			vkCmdDraw(mCommandBuffer, vertexCount, instanceCount, vertexOffset, firstInstance);
		}

		void VulkanCommandBuffer::DrawIndexed(uint32_t indexCount, uint32_t indexOffset, int32_t vertexOffset, uint32_t instanceCount, uint32_t firstInstance)
		{
			vkCmdDrawIndexed(mCommandBuffer, indexCount, instanceCount, indexOffset, vertexOffset, firstInstance);
		}

		void VulkanCommandBuffer::End()
		{
			if(mIsRenderPassStarted == true)
				vkCmdEndRenderPass(mCommandBuffer);

			VkResult res;

			res = vkEndCommandBuffer(mCommandBuffer);
			CheckVkResult(L"VulkanCommandBuffer", L"Cannot end the command buffer", res);
		}

		void VulkanCommandBuffer::Submit(SPtr<BaseRenderQueue>& queue,
			uint32_t waitSemaphoreNum, std::pair<SPtr<BaseRenderSemaphore>, PipelineStageBits>* waitSemaphores,
			uint32_t signalSemaphoreNum, SPtr<BaseRenderSemaphore>* signalSemaphores, SPtr<BaseRenderFence>& waitFence)
		{
			VkResult res;

			Vector<VkSemaphore> wait;
			Vector<VkPipelineStageFlags> waitStage;
			Vector<VkSemaphore> signal;

			wait.resize(waitSemaphoreNum);
			waitStage.resize(waitSemaphoreNum);
			for(uint32_t i = 0; i < waitSemaphoreNum; i++) {
				wait[i] = *DPCast(VulkanSemaphore)(waitSemaphores[i].first);
				waitStage[i] = GetVkPipelineStageFlags(waitSemaphores[i].second);
			}
			signal.resize(signalSemaphoreNum);
			for(uint32_t i = 0; i < signalSemaphoreNum; i++) {
				signal[i] = *DPCast(VulkanSemaphore)(signalSemaphores[i]);
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

			res = vkQueueSubmit(*DPCast(VulkanQueue)(queue), 1, &submitInfo, *DPCast(VulkanFence)(waitFence));
			CheckVkResult(L"VulkanCommandBuffer", L"Cannot submit the command buffer", res);
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
					PrintLogWithSayer(L"VulkanCommandBuffer", L"Unknown PipelineType");
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
			info.flags = 0;
			info.queueFamilyIndex = designatedQueueFamily.mIndex;

			VkResult res;
			res = vkCreateCommandPool(*device, &info, nullptr, &mCommandPool);
			CheckVkResult(L"VulkanCommandPool", L"Cannot create VulkanCommandPool", res);
		}

		VulkanCommandPool::~VulkanCommandPool()
		{
			vkDestroyCommandPool(*mDevice_ref, mCommandPool, nullptr);
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
			commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
			commandBufferAllocateInfo.commandBufferCount = 1;
			res = vkAllocateCommandBuffers(*mDevice_ref, &commandBufferAllocateInfo, &cmd);
			CheckVkResult(L"VulkanCommandPool", L"Cannot allocate VulkanCommandBuffer", res);

			SPtr<VulkanCommandBuffer> cmdBuffer(new VulkanCommandBuffer(mDevice_ref, shared_from_this(), cmd));
			return cmdBuffer;
		}
	}
}