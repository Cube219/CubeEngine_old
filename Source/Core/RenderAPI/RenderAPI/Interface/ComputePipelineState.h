#pragma once

#include "../RenderAPIHeader.h"

namespace cube
{
	namespace render
	{
		struct ComputePipelineStateAttribute : public BaseAttribute
		{
			SPtr<Shader> shader;

			Vector<SPtr<ShaderParametersLayout>> shaderParameterLayouts;
		};

		class ComputePipelineState : public BaseRenderObject
		{
		public:
			ComputePipelineState(const char* debugName) : BaseRenderObject(debugName) {}
			virtual ~ComputePipelineState() {}
		};
	} // namespace render
} // namespace cube
