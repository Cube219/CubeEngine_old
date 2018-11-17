#include "VulkanLogicalDevice.h"

#include "VulkanUtility.h"
#include "VulkanDebug.h"
#include "VulkanPhysicalDevice.h"
#include "BaseRenderAPI_New/Interface/Device.h"

#include "EngineCore/LogWriter.h"

namespace cube
{
	namespace render
	{
		VulkanLogicalDevice::VulkanLogicalDevice(VulkanPhysicalDevice& physicalDevice, const VkPhysicalDeviceFeatures& enabledFeatures,
			const DeviceAttribute& attr) : 
			mParentPhysicalDevice(physicalDevice)
		{
			VkResult res;

			Vector<const char*> extensions;
			extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
			extensions.push_back(VK_KHR_DEDICATED_ALLOCATION_EXTENSION_NAME);

#ifndef _DEBUG // In debug mode, always enable debug layer
			if(attr.enableDebugLayer == true) {
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
			CHECK_VK(res, "Failed to create VkDevice.");

			VulkanDebug::SetObjectName(mDevice, attr.debugName);
		}

		VulkanLogicalDevice::~VulkanLogicalDevice()
		{
			vkDestroyDevice(mDevice, nullptr);
		}

		VkBufferWrapper VulkanLogicalDevice::CreateVkBufferWrapper(const VkBufferCreateInfo& info, const char* debugName)
		{
			VkResult res;

			VkBuffer buf;
			res = vkCreateBuffer(mDevice, &info, nullptr, &buf);
			CHECK_VK(res, "Failed to create VkBuffer '{0}'.", debugName);

			if(debugName != nullptr)
				VulkanDebug::SetObjectName(mDevice, buf, debugName);

			return VkBufferWrapper(buf, shared_from_this());
		}

		VkFenceWrapper VulkanLogicalDevice::CreateVkFenceWrapper(const VkFenceCreateInfo& info, const char* debugName)
		{
			VkResult res;

			VkFence fence;
			res = vkCreateFence(mDevice, &info, nullptr, &fence);
			CHECK_VK(res, "Failed to create VkFence '{0}'.", debugName);

			if(debugName != nullptr)
				VulkanDebug::SetObjectName(mDevice, fence, debugName);

			return VkFenceWrapper(fence, shared_from_this());
		}

		void VulkanLogicalDevice::ReleaseVkObject(VkBufferWrapper&& buffer) const
		{
			vkDestroyBuffer(buffer.GetVkDevice(), buffer.mObject, nullptr);
			// TODO: 이거 나오는지 확인하고 잘 나오면 지움. VkObjectStorage가 잘 작동하는지 확인하기 위함.
			CUBE_LOG(LogType::Info, "Release VkBufferWrapper.");
		}

		void VulkanLogicalDevice::ReleaseVkObject(VkFenceWrapper&& fence) const
		{
			vkDestroyFence(fence.GetVkDevice(), fence.mObject, nullptr);
			// TODO: 이거 나오는지 확인하고 잘 나오면 지움. VkObjectStorage가 잘 작동하는지 확인하기 위함.
			CUBE_LOG(LogType::Info, "Release VkFenceWrapper.");
		}
	} // namespace render
} // namespace cube
