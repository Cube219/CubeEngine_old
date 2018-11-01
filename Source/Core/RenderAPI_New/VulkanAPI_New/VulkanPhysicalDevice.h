#pragma once

#include "VulkanAPIHeader.h"

namespace cube
{
	namespace render
	{
		class VulkanPhysicalDevice
		{
		public:
			VulkanPhysicalDevice(VkPhysicalDevice physicalDevice);
			~VulkanPhysicalDevice();

			VkPhysicalDevice GetHandle() const { return mPhysicalDevice; }

			uint32_t FindQueueFamilyIndex(VkQueueFlags flags) const;

			uint32_t GetMemoryTypeIndex(uint32_t memoryTypeBits, VkMemoryPropertyFlags requirmentFlags);
			VkPhysicalDeviceMemoryProperties GetMemoryProperties() const { return mMemProperties; }

		private:
			VkPhysicalDevice mPhysicalDevice;

			VkPhysicalDeviceProperties mProperties;
			VkPhysicalDeviceFeatures mFeatures;
			VkPhysicalDeviceMemoryProperties mMemProperties;
			Vector<VkQueueFamilyProperties> mQueueFamilyProperties;
			Vector<VkExtensionProperties> mSupportedExtensions;
		};
	} // namespace render
} // namespace cube
