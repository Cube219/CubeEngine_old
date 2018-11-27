#include "CommandListVk.h"

#include "../VulkanUtility.h"
#include "../VulkanCommandListPool.h"
#include "BufferVk.h"
#include "RenderPassVk.h"
#include "RenderTargetVk.h"
#include "GraphicsPipelineStateVk.h"

namespace cube
{
	namespace render
	{
		CommandListVk::CommandListVk(SPtr<VulkanCommandListPool> pool, VkCommandBuffer commandBuffer,
			CommandListUsage usage, Uint32 submitQueueFamilyIndex) :
			CommandList(usage),
			mPool(pool), mCommandBuffer(commandBuffer), mSubmitQueueFamilyIndex(submitQueueFamilyIndex)
		{
		}

		CommandListVk::~CommandListVk()
		{
			mPool->Free(*this);
		}

		void CommandListVk::Begin()
		{
			VkResult res;

			VkCommandBufferBeginInfo beginInfo;
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			beginInfo.pNext = nullptr;
			beginInfo.flags = 0;
			beginInfo.pInheritanceInfo = nullptr;

			res = vkBeginCommandBuffer(mCommandBuffer, &beginInfo);
			CHECK_VK(res, "Failed to begin the command buffer.");
		}

		void CommandListVk::End()
		{
			VkResult res;

			res = vkEndCommandBuffer(mCommandBuffer);
			CHECK_VK(res, "Failed to end the command buffer.");
		}

		void CommandListVk::CopyBuffer(const Buffer& src, Buffer& dst, Uint64 srcOffset, Uint64 dstOffset, Uint64 size)
		{
			VkBufferCopy copy;
			copy.srcOffset = srcOffset;
			copy.dstOffset = dstOffset;
			copy.size = size;

			vkCmdCopyBuffer(mCommandBuffer, DCast(const BufferVk&)(src).GetHandle(), DCast(BufferVk&)(dst).GetHandle(), 1, &copy);
		}

		void CommandListVk::SetRenderPass(SPtr<RenderPass>& renderPass)
		{
			auto renderPassVk = DPCast(RenderPassVk)(renderPass);

			VkRenderPassBeginInfo info;
			info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			info.pNext = nullptr;
			info.renderPass = renderPassVk->GetHandle();
			info.framebuffer = renderPassVk->GetVkFramebuffer();

			Rect2D rect = renderPassVk->GetFramebufferRect2D();
			VkRect2D vkRect = {0, 0, rect.width, rect.height};
			info.renderArea = vkRect;

			auto& renderTargets = renderPassVk->GetRenderTargets();
			Vector<VkClearValue> clearValues(renderTargets.size());
			for(Uint64 i = 0; i < clearValues.size(); i++) {
				clearValues[i] = renderTargets[i]->GetDefaultClearValue();
			}
			info.clearValueCount = SCast(Uint32)(clearValues.size());
			info.pClearValues = clearValues.data();

			vkCmdBeginRenderPass(mCommandBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);
		}

		void CommandListVk::SetPipelineState(SPtr<GraphicsPipelineState>& pipelineState)
		{
			SPtr<GraphicsPipelineStateVk> pipelineVk = DPCast(GraphicsPipelineStateVk)(pipelineState);

			vkCmdBindPipeline(mCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineVk->GetHandle());
		}

		void CommandListVk::SetViewports(Uint32 numViewports, const Viewport* pViewPorts)
		{
			Vector<VkViewport> vp(numViewports);
			for(Uint64 i = 0; i < numViewports; i++) {
				vp[i].x = pViewPorts[i].x;
				vp[i].y = pViewPorts[i].y;
				vp[i].width = pViewPorts[i].width;
				vp[i].height = pViewPorts[i].height;
				vp[i].minDepth = pViewPorts[i].minDepth;
				vp[i].maxDepth = pViewPorts[i].maxDepth;
			}

			vkCmdSetViewport(mCommandBuffer, 0, numViewports, vp.data());
		}

		void CommandListVk::SetScissors(Uint32 numScissors, const Rect2D* pScissors)
		{
			Vector<VkRect2D> sc(numScissors);
			for(Uint64 i = 0; i < numScissors; i++) {
				sc[i].offset.x = pScissors[i].x;
				sc[i].offset.y = pScissors[i].y;
				sc[i].extent.width = pScissors[i].width;
				sc[i].extent.height = pScissors[i].height;
			}

			vkCmdSetScissor(mCommandBuffer, 0, numScissors, sc.data());
		}

		void CommandListVk::BindVertexBuffers(Uint32 startIndex, Uint32 numBuffers, SPtr<Buffer>* pBuffers, Uint32* pOffsets)
		{
			Vector<VkBuffer> bufs(numBuffers);
			Vector<Uint64> offsets(numBuffers);
			for(Uint64 i = 0; i < numBuffers; i++) {
				bufs[i] = DPCast(BufferVk)(pBuffers[i])->GetHandle();
				offsets[i] = pOffsets[i];
			}

			vkCmdBindVertexBuffers(mCommandBuffer, startIndex, numBuffers, bufs.data(), offsets.data());
		}

		void CommandListVk::BindIndexBuffer(SPtr<Buffer> buf, Uint32 offset)
		{
			vkCmdBindIndexBuffer(mCommandBuffer, DPCast(BufferVk)(buf)->GetHandle(), offset, VK_INDEX_TYPE_UINT32);
		}

		void CommandListVk::Draw(Uint32 numVertices, Uint32 baseVertex, Uint32 numInstances, Uint32 baseInstance)
		{
			vkCmdDraw(mCommandBuffer, numVertices, numInstances, baseVertex, baseInstance);
		}

		void CommandListVk::DrawIndexed(Uint32 numIndices, Uint32 baseIndex, Uint32 baseVertex, Uint32 numInstances, Uint32 baseInstance)
		{
			vkCmdDrawIndexed(mCommandBuffer, numIndices, numInstances, baseIndex, baseVertex, baseInstance);
		}

		void CommandListVk::ExecuteCommands(Uint32 numCommandLists, SPtr<CommandList>* cmdLists)
		{
			Vector<VkCommandBuffer> cmds(numCommandLists);
			for(Uint64 i = 0; i < numCommandLists; i++) {
				cmds[i] = DPCast(CommandListVk)(cmdLists[i])->GetHandle();
			}

			vkCmdExecuteCommands(mCommandBuffer, numCommandLists, cmds.data());
		}
	} // namespace render
} // namespace cube
