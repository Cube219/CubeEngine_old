#include "VulkanDevice.h"

#include "VulkanPhysicalDevice.h"
#include "VulkanQueue.h"

namespace cube
{
	namespace core
	{
		VulkanDevice::VulkanDevice()
		{

		}

		VulkanDevice::~VulkanDevice()
		{
			vkDestroyDevice(mDevice, nullptr);
		}

		void VulkanDevice::AddExtension(const char* extensionName)
		{
			mExtensionNames.push_back(extensionName);
		}

		void VulkanDevice::SetFeatures(VulkanPhysicalDeviceFeature feature, bool on, bool isForced)
		{
			if(isForced == false)
				mEnabledFeatures.SetFeature(feature, on);
			else
				mForcedFeatures.SetFeature(feature, on);
		}

		void VulkanDevice::CreateDeviceQueue(VulkanQueueFamily queueFamily, int count, float priorities[])
		{
			VkDeviceQueueCreateInfo info;
			info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			info.pNext = nullptr;
			info.flags = 0;
			info.queueFamilyIndex = queueFamily.mIndex;
			info.queueCount = count;
			info.pQueuePriorities = priorities;

			mDeviceQueueCreateInfos.push_back(info);

			mQueueFamilies.push_back(queueFamily);
		}

		void VulkanDevice::Create(const SPtr<VulkanPhysicalDevice>& physicalDevice)
		{
			mPhysicalDevice_ref = physicalDevice;

			VkResult res;

			VulkanPhysicalDeviceFeatures supportedFeatures(physicalDevice->GetFeatures());

			VulkanPhysicalDeviceFeatures features = (mEnabledFeatures & supportedFeatures) | mForcedFeatures;

			VkDeviceCreateInfo info;
			info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
			info.pNext = nullptr;
			info.flags = 0;
			info.queueCreateInfoCount = SCast(uint32_t)(mDeviceQueueCreateInfos.size());
			info.pQueueCreateInfos = mDeviceQueueCreateInfos.data();
			info.enabledLayerCount = 0; // Deprecated
			info.ppEnabledLayerNames = nullptr; // Deprecated
			info.enabledExtensionCount = SCast(uint32_t)(mExtensionNames.size());
			info.ppEnabledExtensionNames = mExtensionNames.data();
			info.pEnabledFeatures = &SCast(VkPhysicalDeviceFeatures)(features);

			res = vkCreateDevice(*physicalDevice, &info, nullptr, &mDevice);
			CheckVkResult(L"VulkanDevice", L"Cannot create VulkanDevice", res);

			// Get a graphics queue family
			mGraphicsQueueFamily = physicalDevice->GetQueueFamily(VK_QUEUE_GRAPHICS_BIT);
		}

		SPtr<VulkanQueue> VulkanDevice::GetQueue(VkQueueFlags type, uint32_t index)
		{
			for(auto queueFamily : mQueueFamilies) {
				if((queueFamily.mProperties.queueFlags & type) > 0) {
					VkQueue q;
					vkGetDeviceQueue(mDevice, queueFamily.mIndex, index, &q);

					SPtr<VulkanQueue> queue(new VulkanQueue(q));
					// TODO: index 넘어가는거 체크
					return queue;
				}
			}

			PrintlnLogWithSayer(L"VulkanDevice", L"Cannot get a queue");
			return nullptr;
		}

		SPtr<VulkanQueue> VulkanDevice::GetQueue(VulkanQueueFamily queueFamily, uint32_t index)
		{
			VkQueue q;
			vkGetDeviceQueue(mDevice, queueFamily.mIndex, index, &q);

			SPtr<VulkanQueue> queue(new VulkanQueue(q));
			return queue;
		}

		VkDeviceMemory VulkanDevice::AllocateMemory(VkMemoryRequirements require, VkMemoryPropertyFlags memoryPropertyFlags)
		{
			VkResult res;

			VkMemoryAllocateInfo memAllocateInfo = {};
			memAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			memAllocateInfo.pNext = nullptr;
			memAllocateInfo.allocationSize = require.size;
			memAllocateInfo.memoryTypeIndex = GetMemoryTypeIndex(require.memoryTypeBits, memoryPropertyFlags);
			if(memAllocateInfo.memoryTypeIndex == -1) {
				PrintlnLogWithSayer(L"VulkanDevice", L"Cannot find memory type to allocate");
				return nullptr;
			}

			VkDeviceMemory mem;
			res = vkAllocateMemory(mDevice, &memAllocateInfo, nullptr, &mem);
			CheckVkResult(L"VulkanDevice", L"Cannot allocate DeviceMemory", res);
			return mem;
		}

		uint32_t VulkanDevice::GetMemoryTypeIndex(uint32_t memoryTypeBits, VkMemoryPropertyFlags requirmentFlags)
		{
			VkPhysicalDeviceMemoryProperties memProperties = mPhysicalDevice_ref->GetMemProperties();
			for(uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
				if((memoryTypeBits & 1) == 1) {
					if((memProperties.memoryTypes[i].propertyFlags & requirmentFlags) == requirmentFlags) {
						return i;
					}
				}
				memoryTypeBits >>= 1;
			}
			return -1;
		}
	}
}