#include "VulkanPhysicalDevice.h"

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
			// TODO: �� �ڿ�������? ��� ã�ƾ� �ϴ��� �˾ƺ���
			for (size_t i = mQueueFamilyProperties.size() - 1; i >= 0; i--) {
				if ((mQueueFamilyProperties[i].queueFlags & flags) > 0) {
					return i;
				}
			}

			CUBE_LOG(cube::LogType::Error, "Cannot find a queueFamily (VkQueueFlags: {0})", flags);
			return 0;
		}
	} // namespace render
} // namespace cube