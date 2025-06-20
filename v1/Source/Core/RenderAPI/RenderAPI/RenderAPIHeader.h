#pragma once

#include "Base/BaseTypes.h"
#include "Interface/RenderTypes.h"

namespace cube
{
	namespace render
	{
		struct RenderAPIAttribute;
		class RenderAPI;

		struct DeviceAttribute;
		class Device;

		struct BufferAttribute;
		class Buffer;
		class BufferView;

		class CommandList;
		class Fence;
		
		struct GraphicsPipelineStateAttribute;
		class GraphicsPipelineState;
		struct ComputePipelineStateAttribute;
		class ComputePipelineState;

		struct TextureAttribute;
		class Texture;
		struct TextureViewAttribute;
		class TextureView;

		struct SwapChainAttribute;
		class SwapChain;

		struct ShaderAttribute;
		class Shader;
		struct ShaderParametersLayoutAttribute;
		class ShaderParametersLayout;
		class ShaderParameters;

		struct SamplerAttribute;
		class Sampler;

		struct RenderTargetAttribute;
		class RenderTarget;
		struct RenderPassAttribute;
		class RenderPass;
	} // namespace render
} // namespace cube
