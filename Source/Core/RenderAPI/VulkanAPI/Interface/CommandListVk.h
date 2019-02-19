#pragma once

#include "../VulkanAPIHeader.h"

#include "RenderAPI/Interface/CommandList.h"

namespace cube
{
	namespace render
	{
		class CommandListVk final : public CommandList
		{
		public:
			CommandListVk(VulkanCommandListPool& pool, VkCommandBuffer commandBuffer,
				CommandListUsage usage, Uint32 commandPoolIndex, Uint32 submitQueueFamilyIndex, bool isSub,
				const char* debugName);
			virtual ~CommandListVk();

			VkCommandBuffer GetHandle() const { return mCommandBuffer; }

			virtual void Begin() override final;
			virtual void End() override final;
			virtual void Reset() override final;

			virtual void CopyBuffer(const Buffer& src, Buffer& dst, Uint64 srcOffset, Uint64 dstOffset, Uint64 size) override final;

			virtual void SetComputePipelineState(SPtr<ComputePipelineState>& pipelineState) override final;
			virtual void Dispatch(Uint32 groupX, Uint32 groupY, Uint32 groupZ) override final;

			virtual void BindShaderParameters(Uint32 index, SPtr<ShaderParameters>& parameters) override final;

			virtual void SetGraphicsPipelineState(SPtr<GraphicsPipelineState>& pipelineState) override final;

			virtual void SetRenderPass(SPtr<RenderPass>& renderPass, Uint32 renderTargetIndex = 0) override final;

			virtual void SetViewports(Uint32 numViewports, const Viewport* pViewPorts) override final;
			virtual void SetScissors(Uint32 numScissors, const Rect2D* pScissors) override final;

			virtual void BindVertexBuffers(Uint32 startIndex, Uint32 numBuffers, SPtr<Buffer>* pBuffers, Uint32* pOffsets) override final;
			virtual void BindIndexBuffer(SPtr<Buffer> buf, Uint32 offset) override final;

			virtual void Draw(Uint32 numVertices, Uint32 baseVertex, Uint32 numInstances, Uint32 baseInstance) override final;
			virtual void DrawIndexed(Uint32 numIndices, Uint32 baseIndex, Uint32 baseVertex, Uint32 numInstances, Uint32 baseInstance) override final;
			// virtual void DrawIndexedIndirect // TODO: 차후에 구현

			virtual void ExecuteCommands(Uint32 numCommandLists, SPtr<CommandList>* cmdLists) override final;

			Uint32 GetAllocatedCommandPoolIndex() const { return mCommandPoolIndex; }
			Uint32 GetSubmitQueueFamilyIndex() const { return mSubmitQueueFamilyIndex; }

		private:
			friend class VulkanCommandListPool;

			VulkanCommandListPool& mPool;
			Uint32 mCommandPoolIndex;
			Uint32 mSubmitQueueFamilyIndex;

			VkCommandBuffer mCommandBuffer;
			bool mIsSub;

			SPtr<RenderPassVk> mBindedRenderPass = nullptr;
			SPtr<GraphicsPipelineStateVk> mBindedGraphicsPipeline = nullptr;
			SPtr<ComputePipelineStateVk> mBindedComputePipeline = nullptr;
		};
	} // namespace render
} // namespace cube
