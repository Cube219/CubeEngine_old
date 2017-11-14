#pragma once

#include "../VulkanAPIHeader.h"

#include "BaseRenderAPI/Wrapper/BaseRenderGraphicsPipeline.h"

namespace cube
{
	namespace core
	{
		VkPipelineStageFlags GetVkPipelineStageFlags(PipelineStageBits stageBits);

		class VULKAN_API_EXPORT VulkanGraphicsPipeline : public BaseRenderGraphicsPipeline
		{
		public:
			VulkanGraphicsPipeline(const SPtr<VulkanDevice> device, BaseRenderGraphicsPipelineInitializer& initializer);
			virtual ~VulkanGraphicsPipeline();

			VkPipeline GetHandle() const { return mPipeline; }

			const VkPipelineLayout GetLayout() const { return mPipelineLayout; }

		private:
			VkPipeline mPipeline;
			VkPipelineLayout mPipelineLayout;

			SPtr<VulkanDevice> mDevice_ref;
		};
	}
}