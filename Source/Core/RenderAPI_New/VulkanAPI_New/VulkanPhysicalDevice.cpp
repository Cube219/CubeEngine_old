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

		Uint32 VulkanPhysicalDevice::FindQueueFamilyIndex(VkQueueFlags flags, VkQueueFlags prohibitFlags) const
		{
			// TODO: 다른 것들은 어떻게 찾는지 알아보기
			for (Uint32 i = 0; i < mQueueFamilyProperties.size(); i++) {
				if ((mQueueFamilyProperties[i].queueFlags & flags) != 0
					&& (mQueueFamilyProperties[i].queueFlags & prohibitFlags) == 0) {
					return i;
				}
			}

			return Uint32InvalidValue;
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

			flags = flags - ((flags >> 1) & 0x55555555);
			flags = (flags & 0x33333333) + (flags >> 2 & 0x33333333);
			return (((flags + (flags >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
		}
	} // namespace render
} // namespace cube
