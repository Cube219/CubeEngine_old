#pragma once

#include "../RenderAPIHeader.h"

#include "../Utilities/BaseRenderObject.h"

namespace cube
{
	namespace render
	{
		enum class CommandListUsage
		{
			Graphics,
			TransferImmediate,
			TransferDeferred,
			Compute
		};

		struct CommandListAttribute : public BaseAttribute
		{
			CommandListUsage usage;
			Uint32 threadIndex;
			
			bool isSub = false;
		};

		class CommandList : public BaseRenderObject
		{
		public:
			CommandList(CommandListUsage usage, const char* debugName) : BaseRenderObject(debugName), mUsage(usage)
			{}
			virtual ~CommandList() {}

			virtual void Begin() = 0;
			virtual void End() = 0;
			virtual void Reset() = 0;

			virtual void CopyBuffer(const Buffer& src, Buffer& dst, Uint64 srcOffset, Uint64 dstOffset, Uint64 size) = 0;

			virtual void SetComputePipelineState(SPtr<ComputePipelineState>& pipelineState) = 0;
			virtual void Dispatch(Uint32 groupX, Uint32 groupY, Uint32 groupZ) = 0;

			virtual void BindShaderParameters(Uint32 index, SPtr<ShaderParameters>& parameters) = 0;

			virtual void SetGraphicsPipelineState(SPtr<GraphicsPipelineState>& pipelineState) = 0;

			virtual void SetRenderPass(SPtr<RenderPass>& renderPass, Uint32 renderTargetIndex = 0) = 0;

			virtual void SetViewports(Uint32 numViewports, const Viewport* pViewPorts) = 0;
			virtual void SetScissors(Uint32 numScissors, const Rect2D* pScissors) = 0;

			virtual void BindVertexBuffers(Uint32 startIndex, Uint32 numBuffers, SPtr<Buffer>* pBuffers, Uint32* pOffsets) = 0;
			virtual void BindIndexBuffer(SPtr<Buffer> buf, Uint32 offset) = 0;

			virtual void Draw(Uint32 numVertices, Uint32 baseVertex, Uint32 numInstances, Uint32 baseInstance) = 0;
			virtual void DrawIndexed(Uint32 numIndices, Uint32 baseIndex, Uint32 baseVertex, Uint32 numInstances, Uint32 baseInstance) = 0;
			// virtual void DrawIndexedIndirect // TODO: 차후에 구현

			virtual void ExecuteCommands(Uint32 numCommandLists, SPtr<CommandList>* cmdLists) = 0;

			CommandListUsage GetUsage() const { return mUsage; }

		protected:
			CommandListUsage mUsage;
		};
	} // namespace render
} // namespace cube
