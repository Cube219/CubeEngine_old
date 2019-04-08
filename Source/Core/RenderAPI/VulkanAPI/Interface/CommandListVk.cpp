#include "CommandListVk.h"

#include "../VulkanUtility.h"
#include "../VulkanCommandListPool.h"
#include "BufferVk.h"
#include "RenderPassVk.h"
#include "RenderTargetVk.h"
#include "GraphicsPipelineStateVk.h"
#include "ComputePipelineStateVk.h"
#include "ShaderParametersVk.h"
#include "EngineCore/Assertion.h"

namespace cube
{
	namespace render
	{
		CommandListVk::CommandListVk(VulkanCommandListPool& pool, const CommandListAttribute& attr, VkCommandBuffer commandBuffer,
			Uint32 submitQueueFamilyIndex) :
			CommandList(attr),
			mPool(pool), mCommandBuffer(commandBuffer),
			mCommandPoolIndex(attr.threadIndex),
			mSubmitQueueFamilyIndex(submitQueueFamilyIndex)
		{
		}

		CommandListVk::~CommandListVk()
		{
			mPool.Free(*this);
		}

		void CommandListVk::Begin()
		{
			// If it is a sub-command list for graphics, begin will be deferred to SetRenderPass
			// because of pInheritanceInfo
			if(mUsage == CommandListUsage::Graphics && mIsSub == true)
				return;

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
			if(mBindedRenderPass != nullptr && mIsSub == false)
				vkCmdEndRenderPass(mCommandBuffer);

			VkResult res;

			res = vkEndCommandBuffer(mCommandBuffer);
			CHECK_VK(res, "Failed to end the command buffer.");
		}

		void CommandListVk::Reset()
		{
			CHECK(mUsage == CommandListUsage::Graphics || mUsage == CommandListUsage::Compute
				, "Only graphics and compute command list can be reset. Others can be freed to assign nullptr.");

			VkResult res;

			res = vkResetCommandBuffer(mCommandBuffer, 0);
			CHECK_VK(res, "Failed to reset command buffer.");

			mBindedRenderPass = nullptr;
			mBindedGraphicsPipeline = nullptr;
			mBindedComputePipeline = nullptr;
		}

		void CommandListVk::CopyBuffer(const Buffer& src, Buffer& dst, Uint64 srcOffset, Uint64 dstOffset, Uint64 size)
		{
			VkBufferCopy copy;
			copy.srcOffset = srcOffset;
			copy.dstOffset = dstOffset;
			copy.size = size;

			vkCmdCopyBuffer(mCommandBuffer, DCast(const BufferVk&)(src).GetHandle(), DCast(BufferVk&)(dst).GetHandle(), 1, &copy);
		}

		void CommandListVk::SetComputePipelineState(SPtr<ComputePipelineState>& pipelineState)
		{
			CHECK(mUsage == CommandListUsage::Compute, "Only compute command list can set compute pipeline state.");

			mBindedComputePipeline = DPCast(ComputePipelineStateVk)(pipelineState);

			vkCmdBindPipeline(mCommandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, mBindedComputePipeline->GetHandle());
		}

		void CommandListVk::Dispatch(Uint32 groupX, Uint32 groupY, Uint32 groupZ)
		{
			CHECK(mUsage == CommandListUsage::Compute, "Only compute command list can dispatch compute shaders.");

			vkCmdDispatch(mCommandBuffer, groupX, groupY, groupZ);
		}

		void CommandListVk::BindShaderParameters(Uint32 index, SPtr<ShaderParameters>& parameters)
		{
			VkPipelineBindPoint bindPoint;
			VkPipelineLayout layout = VK_NULL_HANDLE;
			switch(mUsage) {
				case CommandListUsage::Graphics:
					bindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
					layout = mBindedGraphicsPipeline->GetLayout();
					break;

				case CommandListUsage::Compute:
					bindPoint = VK_PIPELINE_BIND_POINT_COMPUTE;
					layout = mBindedComputePipeline->GetLayout();
					break;

				case CommandListUsage::TransferImmediate:
				case CommandListUsage::TransferDeferred:
					ASSERTION_FAILED("Only graphics and compute command list can bind shader parameters");
					break;

				default:
					ASSERTION_FAILED("Unknown CommandListUsage ({0})", (int)mUsage);
			}

			DPCast(ShaderParametersVk)(parameters)->BindParameters(mCommandBuffer, bindPoint, layout, index);
		}

		void CommandListVk::SetGraphicsPipelineState(SPtr<GraphicsPipelineState>& pipelineState)
		{
			CHECK(mUsage == CommandListUsage::Graphics, "Only graphics command list can set graphics pipeline state.");

			mBindedGraphicsPipeline = DPCast(GraphicsPipelineStateVk)(pipelineState);

			vkCmdBindPipeline(mCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, mBindedGraphicsPipeline->GetHandle());
		}

		void CommandListVk::SetRenderPass(SPtr<RenderPass>& renderPass, Uint32 renderTargetIndex)
		{
			CHECK(mUsage == CommandListUsage::Graphics, "Only graphics command list can set render pass.");

			mBindedRenderPass = DPCast(RenderPassVk)(renderPass);

			if(mUsage == CommandListUsage::Graphics && mIsSub == true) {
				// If it is a sub-command list for graphics, begin will be deferred to SetRenderPass
				// because of pInheritanceInfo
				VkResult res;

				VkCommandBufferInheritanceInfo inheritance;
				inheritance.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
				inheritance.pNext = nullptr;
				inheritance.renderPass = mBindedRenderPass->GetHandle();
				inheritance.subpass = renderTargetIndex;
				inheritance.framebuffer = mBindedRenderPass->GetVkFramebuffer();
				inheritance.occlusionQueryEnable = VK_FALSE;
				inheritance.queryFlags = 0;
				inheritance.pipelineStatistics = 0;

				VkCommandBufferBeginInfo beginInfo;
				beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
				beginInfo.pNext = nullptr;
				beginInfo.flags = VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
				beginInfo.pInheritanceInfo = &inheritance;

				res = vkBeginCommandBuffer(mCommandBuffer, &beginInfo);
				CHECK_VK(res, "Failed to begin the command buffer for subpass.");
			} else {
				VkRenderPassBeginInfo info;
				info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
				info.pNext = nullptr;
				info.renderPass = mBindedRenderPass->GetHandle();
				info.framebuffer = mBindedRenderPass->GetVkFramebuffer();

				Rect2D rect = mBindedRenderPass->GetFramebufferRect2D();
				VkRect2D vkRect = { 0, 0, rect.width, rect.height };
				info.renderArea = vkRect;

				auto& renderTargets = mBindedRenderPass->GetRenderTargets();
				Vector<VkClearValue> clearValues(renderTargets.size());
				for(Uint64 i = 0; i < clearValues.size(); i++) {
					clearValues[i] = renderTargets[i]->GetDefaultClearValue();
				}
				info.clearValueCount = SCast(Uint32)(clearValues.size());
				info.pClearValues = clearValues.data();

				// In graphics command buffer, set VkSubpassContents VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS
				// because we will use sub-command buffer for recoding draw commands
				if(mUsage == CommandListUsage::Graphics)
					vkCmdBeginRenderPass(mCommandBuffer, &info, VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS);
				else
					vkCmdBeginRenderPass(mCommandBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);
			}
		}

		void CommandListVk::SetViewports(Uint32 numViewports, const Viewport* pViewPorts)
		{
			CHECK(mUsage == CommandListUsage::Graphics, "Only graphics command list can set viewports.");

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
			CHECK(mUsage == CommandListUsage::Graphics, "Only graphics command list can set scissors.");

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
			CHECK(mUsage == CommandListUsage::Graphics, "Only graphics command list can bind vertex buffers.");

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
			CHECK(mUsage == CommandListUsage::Graphics, "Only graphics command list can bind index buffers.");

			vkCmdBindIndexBuffer(mCommandBuffer, DPCast(BufferVk)(buf)->GetHandle(), offset, VK_INDEX_TYPE_UINT32);
		}

		void CommandListVk::Draw(Uint32 numVertices, Uint32 baseVertex, Uint32 numInstances, Uint32 baseInstance)
		{
			CHECK(mUsage == CommandListUsage::Graphics, "Only graphics command list can draw.");

			vkCmdDraw(mCommandBuffer, numVertices, numInstances, baseVertex, baseInstance);
		}

		void CommandListVk::DrawIndexed(Uint32 numIndices, Uint32 baseIndex, Uint32 baseVertex, Uint32 numInstances, Uint32 baseInstance)
		{
			CHECK(mUsage == CommandListUsage::Graphics, "Only graphics command list can draw indexed.");

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
