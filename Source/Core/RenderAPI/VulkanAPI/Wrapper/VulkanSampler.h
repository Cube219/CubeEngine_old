#pragma once

#include "../VulkanAPIHeader.h"

#include "BaseRenderAPI/Wrapper/Sampler.h"

namespace cube
{
	namespace render
	{
		class VULKAN_API_EXPORT VulkanSampler : public Sampler
		{
		public:
			VulkanSampler(const SPtr<VulkanDevice>& device);
			~VulkanSampler();

			VkSampler GetHandle() const { return mSampler; }

		private:
			VkSampler mSampler;

			SPtr<VulkanDevice> mDevice_ref;
		};
	} // namespace render
} // namespace cube
