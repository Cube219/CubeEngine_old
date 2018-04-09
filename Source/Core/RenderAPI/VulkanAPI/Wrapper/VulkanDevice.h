#pragma once

#include "../VulkanAPIHeader.h"

namespace cube
{
	namespace render
	{
		class VulkanDeviceInitializer
		{
		public:
			void AddExtension(const char* extensionName)
			{
				mExtensionNames.push_back(extensionName);
			}

			void AddFeatures(VulkanPhysicalDeviceFeature feature, bool on, bool isForced)
			{
				if(isForced == false)
					mEnabledFeatures.SetFeature(feature, on);
				else
					mForcedFeatures.SetFeature(feature, on);
			}

		private:
			VulkanPhysicalDeviceFeatures mEnabledFeatures;
			VulkanPhysicalDeviceFeatures mForcedFeatures;

			Vector<const char*> mExtensionNames;

			friend class VulkanDevice;
		};

		class VulkanDevice
		{
		public:
			VulkanDevice(const SPtr<VulkanPhysicalDevice>& physicalDevice, VulkanDeviceInitializer& initializer);
			~VulkanDevice();

			VkDevice GetHandle() const { return mDevice; }

			const VkPhysicalDeviceProperties GetProperties() const { return mProperties; };
			const VkPhysicalDeviceFeatures GetFeatures() const { return mFeatures; };
			const VkPhysicalDeviceMemoryProperties GetMemProperties() const { return mMemProperties; };

			VulkanQueueFamily GetGraphicsQueueFamily() const { return mGraphicsQueueFamily; };
			SPtr<VulkanQueue> GetQueue(VkQueueFlags type, uint32_t index);
			SPtr<VulkanQueue> GetQueue(VulkanQueueFamily queueFamily, uint32_t index);
			VulkanQueueFamily GetQueueFamily(VkQueueFlags type);
			const Vector<VulkanQueueFamily>& GetAllQueueFamily() const { return mQueueFamilies; }

			VkDeviceMemory AllocateMemory(VkMemoryRequirements require, VkMemoryPropertyFlags memoryPropertyFlags);

		private:
			uint32_t GetMemoryTypeIndex(uint32_t memoryTypeBits, VkMemoryPropertyFlags requirmentFlags);

			VkDevice mDevice;

			VkPhysicalDeviceProperties mProperties;
			VkPhysicalDeviceFeatures mFeatures;
			VkPhysicalDeviceMemoryProperties mMemProperties;

			Vector<Vector<float>> mDeviceQueuePriorities;

			Vector<VulkanQueueFamily> mQueueFamilies;
			VulkanQueueFamily mGraphicsQueueFamily;

			SPtr<VulkanPhysicalDevice> mPhysicalDevice_ref;
		};
	} // namespace render
} // namespace cube
