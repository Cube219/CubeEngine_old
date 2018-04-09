#pragma once

#include "../VulkanAPIHeader.h"

#include "BaseRenderAPI/Wrapper/GraphicsPipeline.h"

namespace cube
{
	namespace render
	{
		VkPipelineStageFlags GetVkPipelineStageFlags(PipelineStageBits stageBits);

		class VULKAN_API_EXPORT VulkanGraphicsPipeline : public GraphicsPipeline
		{
		public:
			VulkanGraphicsPipeline(const SPtr<VulkanDevice> device, GraphicsPipelineInitializer& initializer);
			virtual ~VulkanGraphicsPipeline();

			VkPipeline GetHandle() const { return mPipeline; }

			const VkPipelineLayout GetLayout() const { return mPipelineLayout; }

		private:
			VkPipeline mPipeline;
			VkPipelineLayout mPipelineLayout;

			SPtr<VulkanDevice> mDevice_ref;
		};
	} // namespace render
} // namespace cube
