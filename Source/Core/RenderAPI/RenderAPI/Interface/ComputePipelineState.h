#pragma once

#include "../RenderAPIHeader.h"

namespace cube
{
	namespace render
	{
		struct ComputePipelineStateAttribute
		{
			SPtr<Shader> shader;

			Vector<SPtr<ShaderParametersLayout>> shaderParameterLayouts;

			const char* debugName = nullptr;
		};

		class ComputePipelineState
		{
		public:
			ComputePipelineState() {}
			virtual ~ComputePipelineState() {}
		};
	} // namespace render
} // namespace cube
