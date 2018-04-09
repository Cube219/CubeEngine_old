#include "VulkanInstance.h"

#include "VulkanPhysicalDevice.h"

namespace cube
{
	namespace render
	{
		VulkanInstance::VulkanInstance(VulkanInstanceInitializer& initializer)
		{
			VkResult res;

			// Set application info
			VkApplicationInfo appInfo = {};
			appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
			appInfo.pNext = nullptr;
			appInfo.pApplicationName = initializer.appName;
			appInfo.applicationVersion = initializer.appVersion;
			appInfo.pEngineName = initializer.engineName;
			appInfo.engineVersion = initializer.engineVersion;
			appInfo.apiVersion = initializer.apiVersion;

			// Set instance create info
			VkInstanceCreateInfo instanceCreateInfo = {};
			instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			instanceCreateInfo.pNext = nullptr;
			instanceCreateInfo.flags = 0;
			instanceCreateInfo.pApplicationInfo = &appInfo;
			instanceCreateInfo.enabledExtensionCount = SCast(uint32_t)(initializer.extensionNames.size());
			instanceCreateInfo.ppEnabledExtensionNames = initializer.extensionNames.data();
			instanceCreateInfo.enabledLayerCount = SCast(uint32_t)(initializer.layerNames.size());
			instanceCreateInfo.ppEnabledLayerNames = initializer.layerNames.data();

			res = vkCreateInstance(&instanceCreateInfo, nullptr, &mInstance);
			CheckVkResult("Cannot create VulkanInstance", res);
		}

		VulkanInstance::~VulkanInstance()
		{
			vkDestroyInstance(mInstance, nullptr);
		}

		const Vector<SPtr<VulkanPhysicalDevice>> VulkanInstance::EnumeratePhysicalDevices() const
		{
			VkResult res;

			Vector<SPtr<VulkanPhysicalDevice>> physicalDevices;
			uint32_t physicalDeviceNum;

			res = vkEnumeratePhysicalDevices(mInstance, &physicalDeviceNum, nullptr);
			CheckVkResult("Cannot enumerate VulkanPhysicalDevices", res);

			if(res == VK_SUCCESS) {
				VkPhysicalDevice* pds = new VkPhysicalDevice[physicalDeviceNum];

				res = vkEnumeratePhysicalDevices(mInstance, &physicalDeviceNum, pds);

				for(uint32_t i = 0; i < physicalDeviceNum; i++) {
					SPtr<VulkanPhysicalDevice> physicalDevice(new VulkanPhysicalDevice(pds[i]));
					physicalDevices.push_back(physicalDevice);
				}
			}

			return physicalDevices;
		}
	} // namespace render
} // namespace cube
