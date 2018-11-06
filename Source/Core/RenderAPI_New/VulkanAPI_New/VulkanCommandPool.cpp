#include "VulkanCommandPool.h"

#include "VulkanUtility.h"
#include "Interface/DeviceVk.h"

namespace cube
{
	namespace render
	{
		VulkanCommandPool::VulkanCommandPool(const SPtr<DeviceVk>& device)
		{
			VkResult res;

			VkCommandPoolCreateInfo info;
			info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
			info.pNext = nullptr;
			info.flags = 0;
			//info.queueFamilyIndex
		}

		VulkanCommandPool::~VulkanCommandPool()
		{
		}
	} // namespace render
} // namespace cube
