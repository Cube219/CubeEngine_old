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

			const VkPhysicalDeviceProperties& GetProperties() const { return mProperties; }

			Uint32 FindQueueFamilyIndex(VkQueueFlags flags, VkQueueFlags prohibitFlags) const;
			const Vector<VkQueueFamilyProperties>& GetQueueFamilyProperties() const { return mQueueFamilyProperties; }

			Uint32 GetMemoryTypeIndex(Uint32 memoryTypeBits, VkMemoryPropertyFlags requiredFlags, VkMemoryPropertyFlags preferrdFlags);
			const VkPhysicalDeviceMemoryProperties& GetMemoryProperties() const { return mMemProperties; }

		private:
			Uint32 CountBits(Uint32 flags);

			VkPhysicalDevice mPhysicalDevice;

			VkPhysicalDeviceProperties mProperties;
			VkPhysicalDeviceFeatures mFeatures;
			VkPhysicalDeviceMemoryProperties mMemProperties;
			Vector<VkQueueFamilyProperties> mQueueFamilyProperties;
			Vector<VkExtensionProperties> mSupportedExtensions;
		};
	} // namespace render
} // namespace cube
