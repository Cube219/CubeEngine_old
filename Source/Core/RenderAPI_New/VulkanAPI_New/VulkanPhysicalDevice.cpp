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

		uint32_t VulkanPhysicalDevice::FindQueueFamilyIndex(VkQueueFlags flags) const
		{
			// TODO: 왜 뒤에서부터? 어떻게 찾아야 하는지 알아보기
			for (size_t i = mQueueFamilyProperties.size() - 1; i >= 0; i--) {
				if ((mQueueFamilyProperties[i].queueFlags & flags) > 0) {
					return SCast(uint32_t)(i);
				}
			}

			ASSERTION_FAILED("Failed to find a queueFamily. (VkQueueFlags: {0})", flags);
			return 0;
		}

		uint32_t VulkanPhysicalDevice::GetMemoryTypeIndex(uint32_t memoryTypeBits, VkMemoryPropertyFlags requirmentFlags)
		{
			// TODO: 원리 알아보기
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
