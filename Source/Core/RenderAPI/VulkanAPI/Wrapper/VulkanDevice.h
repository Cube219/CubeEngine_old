#pragma once

#include "..\VulkanAPIHeader.h"

namespace cube
{
	namespace core
	{
		class VulkanDevice
		{
		public:
			VulkanDevice();
			~VulkanDevice();

			operator VkDevice() const
			{
				return mDevice;
			}

			void AddExtension(const char* extensionName);

			void SetFeatures(VulkanPhysicalDeviceFeature feature, bool on, bool isForced);
			void CreateDeviceQueue(VulkanQueueFamily queueFamily, int count);

			void Create(const SPtr<VulkanPhysicalDevice>& physicalDevice);

			VulkanQueueFamily GetGraphicsQueueFamily() const;
			SPtr<VulkanQueue> GetQueue(VkQueueFlags type, uint32_t index);
			SPtr<VulkanQueue> GetQueue(VulkanQueueFamily queueFamily, uint32_t index);
			VulkanQueueFamily GetQueueFamily(VkQueueFlags type);

			VkDeviceMemory AllocateMemory(VkMemoryRequirements require, VkMemoryPropertyFlags memoryPropertyFlags);

		private:
			uint32_t GetMemoryTypeIndex(uint32_t memoryTypeBits, VkMemoryPropertyFlags requirmentFlags);

			VkDevice mDevice;

			Vector<VkDeviceQueueCreateInfo> mDeviceQueueCreateInfos;
			Vector<Vector<float>> mDeviceQueuePriorities;

			VulkanPhysicalDeviceFeatures mEnabledFeatures;
			VulkanPhysicalDeviceFeatures mForcedFeatures;

			Vector<const char*> mExtensionNames;

			Vector<VulkanQueueFamily> mQueueFamilies;
			VulkanQueueFamily mGraphicsQueueFamily;

			SPtr<VulkanPhysicalDevice> mPhysicalDevice_ref;
		};

		inline VulkanQueueFamily VulkanDevice::GetGraphicsQueueFamily() const
		{
			return mGraphicsQueueFamily;
		}
	}
}