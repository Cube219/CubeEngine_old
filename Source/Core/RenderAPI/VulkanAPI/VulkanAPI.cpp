#include "VulkanAPI.h"

#include "RenderAPI/Utilities/DebugStringHeap.h"
#include "VulkanUtility.h"
#include "Interface/DeviceVk.h"
#include "VulkanPhysicalDevice.h"
#include "VulkanLogicalDevice.h"
#include "VulkanDebug.h"
#include "VulkanTypeConversion.h"
#include "Tools/GLSLTool.h"

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
			mPhysicalDevices.clear();

			VulkanDebug::Free();
			vkDestroyInstance(mInstance, nullptr);

			DebugStringHeap::Disable();

			GLSLTool::Finalize();
		}

		void VulkanAPI::Init(const RenderAPIAttribute& attr)
		{
			TypeConversionInit();

			GLSLTool::Init();

			CreateInstance(attr);

			// Get PhysicalDevices(GPU)
			VkResult res;
			uint32_t physicalDeviceNum;
			res = vkEnumeratePhysicalDevices(mInstance, &physicalDeviceNum, nullptr);
			CHECK_VK(res, "Failed to get physical device number.");
			Vector<VkPhysicalDevice> pd;
			pd.resize(physicalDeviceNum);
			res = vkEnumeratePhysicalDevices(mInstance, &physicalDeviceNum, pd.data());
			CHECK_VK(res, "Failed to enumerate physical devices.");
			for (size_t i = 0; i < pd.size(); i++) {
				mPhysicalDevices.emplace_back(pd[i]);
			}
		}

		SPtr<Device> VulkanAPI::GetDevice(const DeviceAttribute& attr)
		{
			VkPhysicalDeviceFeatures features = {};
			features.tessellationShader = true;
			features.geometryShader = true;
			features.samplerAnisotropy = true;
			features.multiDrawIndirect = true;

			auto logicalDevice = std::make_shared<VulkanLogicalDevice>(mPhysicalDevices[attr.GPUIndex], features, attr);

			auto deviceVk = std::make_shared<DeviceVk>(mInstance, std::move(logicalDevice), attr.debugName);

			// Set physical device debug name
			for(Uint32 i = 0; i < SCast(Uint32)(mPhysicalDevices.size()); i++) {
				U8String debugName = fmt::format("Physical Device {0}", i);
				VulkanDebug::SetObjectName(deviceVk->GetLogicalDevice()->GetHandle(), mPhysicalDevices[i].GetHandle(), debugName.c_str());
			}

			return deviceVk;
		}

		void VulkanAPI::CreateInstance(const RenderAPIAttribute& attr)
		{
			VkResult res;

			Vector<const char*> layers;
			Vector<const char*> extensions;
			extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
#ifdef VK_USE_PLATFORM_WIN32_KHR
			extensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#endif // VK_USE_PLATFORM_WIN32_KHR

			if(attr.enableDebugLayer == true) {
				layers.push_back("VK_LAYER_LUNARG_standard_validation");
				extensions.push_back("VK_EXT_debug_utils");
			}

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
			CHECK_VK(res, "Failed to create VulkanInstance.");

			if(attr.enableDebugLayer == true) {
				VulkanDebug::Setup(mInstance);
				DebugStringHeap::Enable();
			}
		}
	} // namespace render
} // namespace cube
