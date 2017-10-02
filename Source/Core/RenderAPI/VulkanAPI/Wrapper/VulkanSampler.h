#pragma once

#include "../VulkanAPIHeader.h"

#include "BaseRenderAPI/Wrapper/BaseRenderSampler.h"

namespace cube
{
	namespace core
	{
		class VULKAN_API_EXPORT VulkanSampler : public BaseRenderSampler
		{
		public:
			VulkanSampler(const SPtr<VulkanDevice>& device);
			~VulkanSampler();

			operator VkSampler() const
			{
				return mSampler;
			}

		private:
			VkSampler mSampler;

			SPtr<VulkanDevice> mDevice_ref;
		};
	}
}