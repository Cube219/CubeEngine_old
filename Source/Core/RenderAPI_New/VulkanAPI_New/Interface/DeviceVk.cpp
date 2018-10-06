#include "DeviceVk.h"

#include "../VulkanUtility.h"
#include "../VulkanPhysicalDevice.h"

namespace cube
{
	namespace render
	{
		DeviceVk::DeviceVk(SPtr<VulkanPhysicalDevice>& physicalDevice, const VkPhysicalDeviceFeatures& enabledFeatures,
			bool enableDebugLayer)
		{
			VkResult res;

			Vector<const char*> extensions;
			extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
			if (enableDebugLayer == true) {
				extensions.push_back(VK_EXT_DEBUG_MARKER_EXTENSION_NAME);
			}
#ifdef _DEBUG
			else {
				// In debug mode, always enable debug layer
				extensions.push_back(VK_EXT_DEBUG_MARKER_EXTENSION_NAME);
			}
#endif // _DEBUG

			VkDeviceQueueCreateInfo queueInfo = {};
			queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueInfo.flags = 0;
			queueInfo.queueFamilyIndex = physicalDevice->FindQueueFamilyIndex(VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT);
			queueInfo.queueCount = 1; // TODO: 나중에 늘리기?
			const float queuePriority = 1.0f;
			queueInfo.pQueuePriorities = &queuePriority;

			VkDeviceCreateInfo deviceCreateInfo;
			deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
			deviceCreateInfo.pNext = nullptr;
			deviceCreateInfo.flags = 0;
			deviceCreateInfo.queueCreateInfoCount = 1;
			deviceCreateInfo.pQueueCreateInfos = &queueInfo;
			deviceCreateInfo.enabledLayerCount = 0; // Deprecated
			deviceCreateInfo.ppEnabledLayerNames = nullptr; // Deprecated
			deviceCreateInfo.enabledExtensionCount = SCast(uint32_t)(extensions.size());
			deviceCreateInfo.ppEnabledExtensionNames = extensions.data();
			deviceCreateInfo.pEnabledFeatures = &enabledFeatures;

			res = vkCreateDevice(physicalDevice->GetHandle(), &deviceCreateInfo, nullptr, &mDevice);
			CheckVkResult("Failed to create device", res);
		}

		DeviceVk::~DeviceVk()
		{
			vkDestroyDevice(mDevice, nullptr);
		}

		VkDeviceMemory DeviceVk::AllocateMemory(VkMemoryRequirements requirements, VkMemoryPropertyFlags properties)
		{
			return VkDeviceMemory();
		}
	} // namespace render
} // namespace cube
