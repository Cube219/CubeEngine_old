#include "VulkanPhysicalDevice.h"

#include "EngineCore/Assertion.h"
#include "VulkanUtility.h"

namespace cube
{
	namespace render
	{
		VulkanPhysicalDevice::VulkanPhysicalDevice(VkPhysicalDevice physicalDevice) :
			mPhysicalDevice(physicalDevice)
		{
			vkGetPhysicalDeviceProperties(physicalDevice, &mProperties);
			vkGetPhysicalDeviceFeatures(physicalDevice, &mFeatures);
			vkGetPhysicalDeviceMemoryProperties(physicalDevice, &mMemProperties);

			uint32_t queueFamilyNum = 0;
			vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyNum, nullptr);
			mQueueFamilyProperties.resize(queueFamilyNum);
			vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyNum, mQueueFamilyProperties.data());

			uint32_t extensionNum = 0;
			vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionNum, nullptr);
			mSupportedExtensions.resize(extensionNum);
			vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionNum, mSupportedExtensions.data());
		}

		VulkanPhysicalDevice::~VulkanPhysicalDevice()
		{
		}

		Uint32 VulkanPhysicalDevice::FindQueueFamilyIndex(VkQueueFlags flags) const
		{
			// TODO: 왜 뒤에서부터? 어떻게 찾아야 하는지 알아보기
			for (size_t i = mQueueFamilyProperties.size() - 1; i >= 0; i--) {
				if ((mQueueFamilyProperties[i].queueFlags & flags) > 0) {
					return SCast(Uint32)(i);
				}
			}

			ASSERTION_FAILED("Failed to find a queueFamily. (VkQueueFlags: {0})", flags);
			return 0;
		}

		Uint32 VulkanPhysicalDevice::GetMemoryTypeIndex(Uint32 memoryTypeBits, VkMemoryPropertyFlags requiredFlags, VkMemoryPropertyFlags preferredFlags)
		{
			Uint32 memIndex = -1;
			int maxPreferredCount = -1;

			for(Uint32 i = 0; i < mMemProperties.memoryTypeCount; i++) {
				if((memoryTypeBits & 1) == 1) {
					VkMemoryPropertyFlags memProperties = mMemProperties.memoryTypes[i].propertyFlags;

					// Check required flags
					if((memProperties & requiredFlags) == requiredFlags) {
						// Check preferred flags
						Uint32 preferredCount = CountBits(memProperties & preferredFlags);
						if(maxPreferredCount < (int)preferredCount) {
							memIndex = i;
							maxPreferredCount = preferredCount;
						}
					}
				}
				memoryTypeBits >>= 1;
			}

			return memIndex;
		}

		Uint32 VulkanPhysicalDevice::CountBits(Uint32 flags)
		{
			if(flags == 0) return 0;

			Uint32 count = 0;
			for(Uint32 i = 1; i <= (1 << (sizeof(Uint32)*8-1)); i = i << 1) {
				count += flags & i;
			}

			return count;
		}
	} // namespace render
} // namespace cube
