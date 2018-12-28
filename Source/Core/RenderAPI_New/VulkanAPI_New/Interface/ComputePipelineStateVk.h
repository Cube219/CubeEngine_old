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

			VkPipeline GetHandle() const { return mPipeline.mObject; }

		private:
			VkPipelineWrapper mPipeline;
			VkPipelineLayoutWrapper mLayout;
		};
	} // namespace render
} // namespace cube
