#include "DeviceVk.h"

#include "../VulkanUtility.h"
#include "../VulkanDebug.h"
#include "../VulkanPhysicalDevice.h"
#include "../VulkanMemoryManager.h"
#include "BaseRenderAPI_New/Interface/Buffer.h"
#include "BufferVk.h"

namespace cube
{
	namespace render
	{
		DeviceVk::DeviceVk(VulkanPhysicalDevice& physicalDevice, const VkPhysicalDeviceFeatures& enabledFeatures,
			const DeviceAttribute& attr) :
			mParentPhysicalDevice(physicalDevice)
		{
			VkResult res;

			Vector<const char*> extensions;
			extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
			extensions.push_back(VK_KHR_DEDICATED_ALLOCATION_EXTENSION_NAME);

#ifndef _DEBUG // In debug mode, always enable debug layer
			if (attr.enableDebugLayer == true) {
#endif // _DEBUG
				// Deprecated. Use "VK_EXT_debug_utils" in instance instead.
				// extensions.push_back(VK_EXT_DEBUG_MARKER_EXTENSION_NAME);
#ifndef _DEBUG // In debug mode, always enable debug layer
			}
#endif // _DEBUG

			VkDeviceQueueCreateInfo queueInfo = {};
			queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueInfo.flags = 0;
			queueInfo.queueFamilyIndex = physicalDevice.FindQueueFamilyIndex(VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT);
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

			res = vkCreateDevice(physicalDevice.GetHandle(), &deviceCreateInfo, nullptr, &mDevice);
			CheckVkResult("Failed to create device.", res);

			VulkanDebug::SetObjectName(mDevice, attr.debugName);

			mMemoryManager = std::make_unique<VulkanMemoryManager>(*this,
				256 * 1024 * 1024, // 256MiB
				32 * 1024 * 1024  // 32MiB
			);
		}

		DeviceVk::~DeviceVk()
		{
			vkDestroyDevice(mDevice, nullptr);
		}

		SPtr<Buffer> DeviceVk::CreateBuffer(const BufferAttribute& attr)
		{
			return std::make_shared<BufferVk>(shared_from_this(), attr);
			// RN: 왜 BUfferVk에서 첫 번째 인수에 const를 붙여야 하는가?
		}
	} // namespace render
} // namespace cube
