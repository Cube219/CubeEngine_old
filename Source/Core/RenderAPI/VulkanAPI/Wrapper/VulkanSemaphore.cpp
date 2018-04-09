#include "VulkanSemaphore.h"

#include "VulkanDevice.h"

namespace cube
{
	namespace render
	{
		VulkanSemaphore::VulkanSemaphore(SPtr<VulkanDevice>& device) :
			mDevice_ref(device)
		{
			VkSemaphoreCreateInfo info;
			info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
			info.pNext = nullptr;
			info.flags = 0;

			vkCreateSemaphore(device->GetHandle(), &info, nullptr, &mSemaphore);
		}

		VulkanSemaphore::~VulkanSemaphore()
		{
			vkDestroySemaphore(mDevice_ref->GetHandle(), mSemaphore, nullptr);
		}
	} // namespace render
} // namespace cube
