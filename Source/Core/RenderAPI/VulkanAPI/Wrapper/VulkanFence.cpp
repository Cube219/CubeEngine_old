#include "VulkanFence.h"

#include "VulkanDevice.h"

namespace cube
{
	namespace render
	{
		VulkanFence::VulkanFence(SPtr<VulkanDevice>& device) :
			mDevice_ref(device)
		{
			VkResult res;

			VkFenceCreateInfo info;
			info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
			info.pNext = nullptr;
			info.flags = 0;

			res = vkCreateFence(device->GetHandle(), &info, nullptr, &mFence);
			CheckVkResult("Cannot create a VulkanFence", res);
		}

		VulkanFence::~VulkanFence()
		{
			vkDestroyFence(mDevice_ref->GetHandle(), mFence, nullptr);
		}

		bool VulkanFence::Wait(uint64_t timeout)
		{
			VkResult res;

			res = vkWaitForFences(mDevice_ref->GetHandle(), 1, &mFence, VK_TRUE, timeout);

			if(res == VK_TIMEOUT) {
				return false;
			}

			CheckVkResult("Error occured waiting for the fance", res);

			return true;
		}

		void VulkanFence::Reset()
		{
			vkResetFences(mDevice_ref->GetHandle(), 1, &mFence);
		}
	} // namespace render
} // namespace cube
