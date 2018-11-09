#include "DeviceVk.h"

#include "../VulkanUtility.h"
#include "../VulkanDebug.h"
#include "../VulkanPhysicalDevice.h"
#include "../VulkanMemoryManager.h"
#include "../VulkanQueueManager.h"
#include "../VulkanFencePool.h"
#include "../VulkanSemaphorePool.h"
#include "BaseRenderAPI_New/Interface/Buffer.h"
#include "BufferVk.h"
#include "CommandListVk.h"

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

			auto& queueFamilyProps = physicalDevice.GetQueueFamilyProperties();

			// Put all queue familiy of the physical device into device queue
			Vector<VkDeviceQueueCreateInfo> queueInfos(queueFamilyProps.size());
			Vector<Vector<float>> queuePriorities(queueFamilyProps.size());

			for(Uint32 i = 0; i < (Uint32)queueFamilyProps.size(); i++) {
				queuePriorities[i].resize(queueFamilyProps[i].queueCount, 1.0f);

				VkDeviceQueueCreateInfo info;
				info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
				info.pNext = nullptr;
				info.flags = 0;
				info.queueFamilyIndex = i;
				info.queueCount = queueFamilyProps[i].queueCount;
				info.pQueuePriorities = queuePriorities[i].data();

				queueInfos[i] = info;
			}

			// Create device
			VkDeviceCreateInfo deviceCreateInfo;
			deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
			deviceCreateInfo.pNext = nullptr;
			deviceCreateInfo.flags = 0;
			deviceCreateInfo.queueCreateInfoCount = SCast(Uint32)(queueInfos.size());
			deviceCreateInfo.pQueueCreateInfos = queueInfos.data();
			deviceCreateInfo.enabledLayerCount = 0; // Deprecated
			deviceCreateInfo.ppEnabledLayerNames = nullptr; // Deprecated
			deviceCreateInfo.enabledExtensionCount = SCast(Uint32)(extensions.size());
			deviceCreateInfo.ppEnabledExtensionNames = extensions.data();
			deviceCreateInfo.pEnabledFeatures = &enabledFeatures;

			res = vkCreateDevice(physicalDevice.GetHandle(), &deviceCreateInfo, nullptr, &mDevice);
			CheckVkResult("Failed to create device.", res);

			VulkanDebug::SetObjectName(mDevice, attr.debugName);

			mMemoryManager = std::make_unique<VulkanMemoryManager>(*this,
				256 * 1024 * 1024, // 256MiB
				32 * 1024 * 1024  // 32MiB
			);

			mFencePool = std::make_unique<VulkanFencePool>(*this);
			mSemaphorePool = std::make_unique<VulkanSemaphorePool>(*this);
			mQueueManager = std::make_unique<VulkanQueueManager>(*this, physicalDevice, *mFencePool, *mSemaphorePool);
		}

		DeviceVk::~DeviceVk()
		{
			mMemoryManager = nullptr;
			mFencePool = nullptr;

			vkDestroyDevice(mDevice, nullptr);
		}

		SPtr<Buffer> DeviceVk::CreateBuffer(const BufferAttribute& attr)
		{
			return std::make_shared<BufferVk>(shared_from_this(), attr);
		}

		void DeviceVk::SubmitCommandList(SPtr<CommandList>& commandList)
		{
			// TODO: 여러 Queue에 동시에 Submit?
			// RN: QueueSubmitManager? 같은걸 만들어야 할 듯
			// DeviceVk에서는 그냥 Queue리스트만 제공
			SPtr<CommandListVk> commandListVk = DPCast(CommandListVk)(commandList);
		}
	} // namespace render
} // namespace cube
