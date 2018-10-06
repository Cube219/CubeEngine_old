#include "VulkanDevice.h"

#include "EngineCore/Assertion.h"
#include "VulkanPhysicalDevice.h"
#include "VulkanQueue.h"

namespace cube
{
	namespace render
	{
		VulkanDevice::VulkanDevice(const SPtr<VulkanPhysicalDevice>& physicalDevice, VulkanDeviceInitializer& initializer) :
			mPhysicalDevice_ref(physicalDevice)
		{
			VkResult res;

			// Feature
			VkPhysicalDeviceFeatures tempVkFeatures;
			vkGetPhysicalDeviceFeatures(physicalDevice->GetHandle(), &tempVkFeatures);
			VulkanPhysicalDeviceFeatures supportedFeatures = VulkanPhysicalDeviceFeatures(tempVkFeatures);

			VulkanPhysicalDeviceFeatures features = (initializer.mEnabledFeatures & supportedFeatures) | initializer.mForcedFeatures;

			// Properties
			vkGetPhysicalDeviceProperties(physicalDevice->GetHandle(), &mProperties);
			vkGetPhysicalDeviceMemoryProperties(physicalDevice->GetHandle(), &mMemProperties);

			// Get queue families of the physical device
			uint32_t physicalQueueFamilyNum = 0;
			vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice->GetHandle(), &physicalQueueFamilyNum, nullptr);

			Vector<VkQueueFamilyProperties> physicalQueueFamilyProps;
			physicalQueueFamilyProps.resize(physicalQueueFamilyNum);
			vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice->GetHandle(), &physicalQueueFamilyNum, physicalQueueFamilyProps.data());

			// Put all queue familiy of the physical device into device queue
			Vector<VkDeviceQueueCreateInfo> deviceQueueCreateInfos;
			deviceQueueCreateInfos.resize(physicalQueueFamilyNum);

			uint64_t prioritiesNum = 0;
			for(uint32_t i = 0; i < physicalQueueFamilyNum; i++) {
				VulkanQueueFamily f;
				f.mIndex = i;
				f.mProperties = physicalQueueFamilyProps[i];
				mQueueFamilies.push_back(f);

				VkDeviceQueueCreateInfo info;
				info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
				info.pNext = nullptr;
				info.flags = 0;
				info.queueFamilyIndex = i;
				info.queueCount = physicalQueueFamilyProps[i].queueCount;

				deviceQueueCreateInfos[i] = info;

				prioritiesNum += info.queueCount;
			}

			Vector<float> priorities;
			priorities.resize(prioritiesNum, 1.0f);
			float* prioritiesPtr = priorities.data();
			for(auto& info : deviceQueueCreateInfos) {
				info.pQueuePriorities = prioritiesPtr;
				prioritiesPtr += info.queueCount;
			}

			VkDeviceCreateInfo info;
			info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
			info.pNext = nullptr;
			info.flags = 0;
			info.queueCreateInfoCount = SCast(uint32_t)(deviceQueueCreateInfos.size());
			info.pQueueCreateInfos = deviceQueueCreateInfos.data();
			info.enabledLayerCount = 0; // Deprecated
			info.ppEnabledLayerNames = nullptr; // Deprecated
			info.enabledExtensionCount = SCast(uint32_t)(initializer.mExtensionNames.size());
			info.ppEnabledExtensionNames = initializer.mExtensionNames.data();
			info.pEnabledFeatures = &SCast(VkPhysicalDeviceFeatures)(features);

			res = vkCreateDevice(physicalDevice->GetHandle(), &info, nullptr, &mDevice);
			CheckVkResult("Cannot create VulkanDevice", res);

			// Get a graphics queue family
			mGraphicsQueueFamily = GetQueueFamily(VK_QUEUE_GRAPHICS_BIT);
		}

		VulkanDevice::~VulkanDevice()
		{
			vkDestroyDevice(mDevice, nullptr);
		}

		SPtr<VulkanQueue> VulkanDevice::GetQueue(VkQueueFlags type, uint32_t index)
		{
			auto family = GetQueueFamily(type);

			return GetQueue(family, index);
		}

		SPtr<VulkanQueue> VulkanDevice::GetQueue(VulkanQueueFamily queueFamily, uint32_t index)
		{
			VkQueue q;
			vkGetDeviceQueue(mDevice, queueFamily.mIndex, index, &q);

			SPtr<VulkanQueue> queue(new VulkanQueue(q, queueFamily.mProperties.queueFlags, index));
			return queue;
		}

		VulkanQueueFamily VulkanDevice::GetQueueFamily(VkQueueFlags type)
		{
			for(size_t i = mQueueFamilies.size() - 1; i >= 0; i--) {
				if((mQueueFamilies[i].mProperties.queueFlags & type) > 0) {
					return mQueueFamilies[i];
				}
			}

			ASSERTION_FAILED("Failed to find a queueFamily. (VkQueueFlags: {0})", type);
			return {};
		}

		VkDeviceMemory VulkanDevice::AllocateMemory(VkMemoryRequirements require, VkMemoryPropertyFlags memoryPropertyFlags)
		{
			VkResult res;

			VkMemoryAllocateInfo memAllocateInfo = {};
			memAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			memAllocateInfo.pNext = nullptr;
			memAllocateInfo.allocationSize = require.size;
			memAllocateInfo.memoryTypeIndex = GetMemoryTypeIndex(require.memoryTypeBits, memoryPropertyFlags);
			CHECK(memAllocateInfo.memoryTypeIndex != -1, "Failed to find memory type to allocate (MemoryTypeBits: {0} / VkMemoryPropertyFlags: {1})",
				require.memoryTypeBits, memoryPropertyFlags);

			VkDeviceMemory mem;
			res = vkAllocateMemory(mDevice, &memAllocateInfo, nullptr, &mem);
			CheckVkResult("Failed to allocate DeviceMemory", res);
			return mem;
		}

		uint32_t VulkanDevice::GetMemoryTypeIndex(uint32_t memoryTypeBits, VkMemoryPropertyFlags requirmentFlags)
		{
			for(uint32_t i = 0; i < mMemProperties.memoryTypeCount; i++) {
				if((memoryTypeBits & 1) == 1) {
					if((mMemProperties.memoryTypes[i].propertyFlags & requirmentFlags) == requirmentFlags) {
						return i;
					}
				}
				memoryTypeBits >>= 1;
			}
			return -1;
		}
	} // namespace render
} // namespace cube
