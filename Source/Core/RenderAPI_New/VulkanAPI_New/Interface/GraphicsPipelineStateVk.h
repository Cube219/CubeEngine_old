#pragma once

#include "../VulkanAPIHeader.h"

#include "BaseRenderAPI_New/Interface/GraphicsPipelineState.h"

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

		private:
			VkPipelineWrapper mPipeline;
		};
	} // namespace render
} // namespace cube
