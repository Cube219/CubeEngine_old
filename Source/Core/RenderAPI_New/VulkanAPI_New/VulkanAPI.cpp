#include "VulkanAPI.h"

#include "VulkanUtility.h"

namespace cube
{
	namespace render
	{
		VULKAN_API_EXPORT RenderAPI* GetAPI()
		{
			return new VulkanAPI();
		}

		VulkanAPI::~VulkanAPI()
		{
			vkDestroyInstance(mInstance, nullptr);
		}

		void VulkanAPI::Init(const RenderAPIAttribute& attr)
		{
			CreateInstance(attr);
		}

		SPtr<Device> VulkanAPI::GetDevice(const DeviceAttribute& attr)
		{
			return SPtr<Device>();
		}

		void VulkanAPI::CreateInstance(const RenderAPIAttribute& attr)
		{
			VkResult res;

			Vector<const char*> layers;
			if (attr.enableDebugLayer == true) {
				layers.push_back("VK_LAYER_LUNARG_standard_validation");
			}
#ifdef _DEBUG
			else {
				// In debug mode, always enable debug layer
				layers.push_back("VK_LAYER_LUNARG_standard_validation");
			}
#endif // _DEBUG

			Vector<const char*> extensions;
			extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
#ifdef VK_USE_PLATFORM_WIN32_KHR
			extensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#endif // VK_USE_PLATFORM_WIN32_KHR

			VkApplicationInfo appInfo = {};
			appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
			appInfo.pNext = nullptr;
			appInfo.pApplicationName = nullptr;
			appInfo.applicationVersion = 0;
			appInfo.pEngineName = "Cube Engine";
			appInfo.engineVersion = 0;
			appInfo.apiVersion = VK_API_VERSION_1_0;

			VkInstanceCreateInfo instanceCreateInfo = {};
			instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			instanceCreateInfo.pNext = nullptr;
			instanceCreateInfo.flags = 0;
			instanceCreateInfo.pApplicationInfo = &appInfo;
			instanceCreateInfo.enabledExtensionCount = SCast(uint32_t)(extensions.size());
			instanceCreateInfo.ppEnabledExtensionNames = extensions.data();
			instanceCreateInfo.enabledLayerCount = SCast(uint32_t)(layers.size());
			instanceCreateInfo.ppEnabledLayerNames = layers.data();

			res = vkCreateInstance(&instanceCreateInfo, nullptr, &mInstance);
			CheckVkResult("Cannot create VulkanInstance", res);
		}
	} // namespace render
} // namespace cube
