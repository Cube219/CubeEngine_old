#pragma once

#include "../VulkanAPIHeader.h"

#include "RenderAPI/Interface/GraphicsPipelineState.h"

#include "../VkObject.h"

namespace cube
{
	namespace render
	{
		class GraphicsPipelineStateVk final : public GraphicsPipelineState
		{
		public:
			GraphicsPipelineStateVk(DeviceVk& device, const GraphicsPipelineStateAttribute& attr);
			virtual ~GraphicsPipelineStateVk();

			VkPipeline GetHandle() const { return mPipeline.mObject; }
			VkPipelineLayout GetLayout() const { return mLayout.mObject; }

		private:
			VkPipelineWrapper mPipeline;
			VkPipelineLayoutWrapper mLayout;
		};
	} // namespace render
} // namespace cube
