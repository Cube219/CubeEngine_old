#pragma once

#include "../VulkanAPIHeader.h"

#include "BaseRenderAPI/Wrapper/Semaphore.h"

namespace cube
{
	namespace render
	{
		class VULKAN_API_EXPORT VulkanSemaphore : public Semaphore
		{
		public:
			VulkanSemaphore(SPtr<VulkanDevice>& device);
			virtual ~VulkanSemaphore();

			VkSemaphore GetHandle() const { return mSemaphore; }

		private:
			VkSemaphore mSemaphore;

			SPtr<VulkanDevice> mDevice_ref;
		};
	} // namespace render
} // namespace cube
