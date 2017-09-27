#pragma once

#include "..\VulkanAPIHeader.h"

#include "BaseRenderAPI\Wrapper\BaseRenderFence.h"

namespace cube
{
	namespace core
	{
		class VULKAN_API_EXPORT VulkanFence : public BaseRenderFence
		{
		public:
			VulkanFence(SPtr<VulkanDevice>& device);
			virtual ~VulkanFence();

			operator VkFence() const
			{
				return mFence;
			}

			bool Wait(uint64_t timeout) override;
			void Reset() override;

		private:
			VkFence mFence;

			SPtr<VulkanDevice> mDevice_ref;
		};
	}
}