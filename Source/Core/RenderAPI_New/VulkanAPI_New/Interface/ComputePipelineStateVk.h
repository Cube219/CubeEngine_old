#pragma once

#include "../VulkanAPIHeader.h"

#include "BaseRenderAPI_New/Interface/ComputePipelineState.h"

#include "../VkObject.h"

namespace cube
{
	namespace render
	{
		class ComputePipelineStateVk final : public ComputePipelineState
		{
		public:
			ComputePipelineStateVk(DeviceVk& device, const ComputePipelineStateAttribute& attr);
			virtual ~ComputePipelineStateVk();

		private:
			VkPipelineWrapper mPipeline;
		};
	} // namespace render
} // namespace cube
