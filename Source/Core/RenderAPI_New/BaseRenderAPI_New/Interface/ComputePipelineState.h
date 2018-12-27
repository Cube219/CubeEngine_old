#pragma once

#include "../RenderAPIHeader.h"

namespace cube
{
	namespace render
	{
		struct ComputePipelineStateAttribute
		{
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
