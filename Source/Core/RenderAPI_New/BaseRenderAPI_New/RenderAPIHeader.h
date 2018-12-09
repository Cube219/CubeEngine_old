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

		class Fence;
		
		struct GraphicsPipelineStateAttribute;
		class GraphicsPipelineState;

		struct TextureAttribute;
		class Texture;
		struct TextureViewAttribute;
		class TextureView;

		struct SwapChainAttribute;
		class SwapChain;

		class Shader;
		class ShaderVariable;
		class ShaderVariablesBinder;

		class Sampler;

		class CommandList;

		struct RenderTargetAttribute;
		class RenderTarget;
		struct RenderPassAttribute;
		class RenderPass;
	} // namespace render
} // namespace cube
