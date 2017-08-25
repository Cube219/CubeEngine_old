#include "VulkanInstance.h"

#include "VulkanPhysicalDevice.h"

namespace cube
{
	namespace core
	{
		VulkanInstance::VulkanInstance()
		{
		}

		VulkanInstance::~VulkanInstance()
		{
			mLayerNames.clear();
			mExtensionNames.clear();

			vkDestroyInstance(mInstance, nullptr);
		}

		void VulkanInstance::AddLayer(const char* layerName)
		{
			mLayerNames.push_back(layerName);
		}

		void VulkanInstance::AddExtension(const char* extensionName)
		{
			mExtensionNames.push_back(extensionName);
		}

		void VulkanInstance::Create()
		{
			VkResult res;

			// Set application info
			VkApplicationInfo appInfo = {};
			appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
			appInfo.pNext = nullptr;
			appInfo.pApplicationName = mApplicationName;
			appInfo.applicationVersion = mApplicationVersion;
			appInfo.pEngineName = mEngineName;
			appInfo.engineVersion = mEngineVersion;
			appInfo.apiVersion = mApiVersion;

			// Set instance create info
			VkInstanceCreateInfo instanceCreateInfo = {};
			instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			instanceCreateInfo.pNext = nullptr;
			instanceCreateInfo.flags = 0;
			instanceCreateInfo.pApplicationInfo = &appInfo;
			instanceCreateInfo.enabledExtensionCount = SCast(uint32_t)(mExtensionNames.size());
			instanceCreateInfo.ppEnabledExtensionNames = mExtensionNames.data();
			instanceCreateInfo.enabledLayerCount = SCast(uint32_t)(mLayerNames.size());
			instanceCreateInfo.ppEnabledLayerNames = mLayerNames.data();

			res = vkCreateInstance(&instanceCreateInfo, nullptr, &mInstance);
			CheckVkResult(L"VulkanInstance", L"Cannot create VulkanInstance", res);
		}

		const Vector<SPtr<VulkanPhysicalDevice>> VulkanInstance::EnumeratePhysicalDevices() const
		{
			VkResult res;

			Vector<SPtr<VulkanPhysicalDevice>> physicalDevices;
			uint32_t physicalDeviceNum;

			res = vkEnumeratePhysicalDevices(mInstance, &physicalDeviceNum, nullptr);
			CheckVkResult(L"VulkanInstance", L"Cannot enumerate VulkanPhysicalDevices", res);

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
	}
}