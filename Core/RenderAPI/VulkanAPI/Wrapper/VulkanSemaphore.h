#pragma once

#include "..\VulkanAPIHeader.h"

#include "BaseRenderAPI\Wrapper\BaseRenderSemaphore.h"

namespace cube
{
	namespace core
	{
		class VULKAN_API_EXPORT VulkanSemaphore : public BaseRenderSemaphore
		{
		public:
			VulkanSemaphore(SPtr<VulkanDevice>& device);
			virtual ~VulkanSemaphore();

			operator VkSemaphore() const
			{
				return mSemaphore;
			}

		private:
			VkSemaphore mSemaphore;

			SPtr<VulkanDevice> mDevice_ref;
		};
	}
}