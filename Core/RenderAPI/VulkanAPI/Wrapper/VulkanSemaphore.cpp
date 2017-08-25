#include "VulkanSemaphore.h"

#include "VulkanDevice.h"

namespace cube
{
	namespace core
	{
		VulkanSemaphore::VulkanSemaphore(SPtr<VulkanDevice>& device) :
			mDevice_ref(device)
		{
			VkSemaphoreCreateInfo info;
			info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
			info.pNext = nullptr;
			info.flags = 0;

			vkCreateSemaphore(*device, &info, nullptr, &mSemaphore);
		}

		VulkanSemaphore::~VulkanSemaphore()
		{
			vkDestroySemaphore(*mDevice_ref, mSemaphore, nullptr);
		}
	}
}