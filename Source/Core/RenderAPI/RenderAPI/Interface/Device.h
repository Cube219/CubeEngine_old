#pragma once

#include "../RenderAPIHeader.h"

#include "../Utilities/BaseRenderObject.h"
#include "CommandList.h"

namespace cube
{
	namespace render
	{
		struct DeviceAttribute : public BaseAttribute
		{
			uint32_t GPUIndex;
			bool enableDebugLayer;
		};

		class Device : public BaseRenderObject
		{
		public:
			Device(const char* debugName) : BaseRenderObject(debugName) {}
			virtual ~Device() {}

			virtual SPtr<Buffer> CreateBuffer(const BufferAttribute& attr) = 0;
			virtual SPtr<Texture> CreateTexture(const TextureAttribute& attr) = 0;
			virtual SPtr<SwapChain> CreateSwapChain(const SwapChainAttribute& attr) = 0;
			virtual SPtr<Sampler> CreateSampler(const SamplerAttribute& attr) = 0;

			virtual SPtr<RenderTarget> CreateRenderTarget(const RenderTargetAttribute& attr) = 0;
			virtual SPtr<RenderPass> CreateRenderPass(const RenderPassAttribute& attr) = 0;

			virtual SPtr<ShaderParametersLayout> CreateShaderParametersLayout(const ShaderParametersLayoutAttribute& attr) = 0;
			virtual SPtr<Shader> CreateShader(const ShaderAttribute& attr) = 0;
			virtual SPtr<GraphicsPipelineState> CreateGraphicsPipelineState(const GraphicsPipelineStateAttribute& attr) = 0;
			virtual SPtr<ComputePipelineState> CreateComputePipelineState(const ComputePipelineStateAttribute& attr) = 0;

			virtual SPtr<Fence> GetFence(const char* debugName = "") = 0;

			virtual SPtr<CommandList> GetCommandList(const CommandListAttribute& attr) = 0;
			virtual void SubmitCommandList(SPtr<CommandList>& commandList) = 0;
			virtual SPtr<Fence> SubmitCommandListWithFence(SPtr<CommandList>& commandList) = 0;

			virtual void StartFrame() = 0;
		};
	} // namespace render
} // namespace cube
