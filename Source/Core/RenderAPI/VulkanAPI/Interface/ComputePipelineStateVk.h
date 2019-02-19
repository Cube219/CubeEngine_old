#pragma once

#include "../VulkanAPIHeader.h"

#include "RenderAPI/Interface/ComputePipelineState.h"

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
			VkPipelineLayout GetLayout() const { return mLayout.mObject; }

		private:
			VkPipelineWrapper mPipeline;
			VkPipelineLayoutWrapper mLayout;
		};
	} // namespace render
} // namespace cube
