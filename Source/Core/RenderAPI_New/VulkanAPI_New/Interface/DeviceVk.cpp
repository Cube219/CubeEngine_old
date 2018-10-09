#include "DeviceVk.h"

#include "../VulkanUtility.h"
#include "../VulkanPhysicalDevice.h"

namespace cube
{
	namespace render
	{
		DeviceVk::DeviceVk(SPtr<VulkanPhysicalDevice>& physicalDevice, const VkPhysicalDeviceFeatures& enabledFeatures,
			bool enableDebugLayer) : 
			mPhysicalDevice(physicalDevice)
		{
			VkResult res;

			Vector<const char*> extensions;
			extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

#ifndef _DEBUG // In debug mode, always enable debug layer
			if (enableDebugLayer == true) {
#endif // _DEBUG
				// Deprecated. Use "VK_EXT_debug_utils" in instance instead.
				// extensions.push_back(VK_EXT_DEBUG_MARKER_EXTENSION_NAME);
#ifndef _DEBUG // In debug mode, always enable debug layer
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
			VkResult res;

			VkMemoryAllocateInfo memAllocateInfo = {};
			memAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			memAllocateInfo.pNext = nullptr;
			memAllocateInfo.allocationSize = requirements.size;
			memAllocateInfo.memoryTypeIndex = mPhysicalDevice->GetMemoryTypeIndex(requirements.memoryTypeBits, properties);

			VkDeviceMemory mem;
			res = vkAllocateMemory(mDevice, &memAllocateInfo, nullptr, &mem);
			CheckVkResult("Failed to allocate DeviceMemory.", res);

			return mem;
		}
	} // namespace render
} // namespace cube
