#include "VulkanDebug.h"

#include "VulkanUtility.h"
#include "EngineCore/Assertion.h"
#include "PlatformDebugUtility.h"

namespace cube
{
	namespace render
	{
		VkInstance VulkanDebug::mInstance;
		VkDebugUtilsMessengerEXT VulkanDebug::mMessenger;

		void VulkanDebug::Setup(VkInstance instance)
		{
			mInstance = instance;

			VkResult res;

			VkDebugUtilsMessengerCreateInfoEXT info;
			info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
			info.pNext = nullptr;
			info.flags = 0;
			info.messageSeverity =
				VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | 
				VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
			info.messageType = 
				VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | 
				VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | 
				VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
			info.pfnUserCallback = VulkanDebug::MessageCallback;
			info.pUserData = nullptr;

			auto func = RCast(PFN_vkCreateDebugUtilsMessengerEXT)(vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT"));
			res = func(instance, &info, nullptr, &mMessenger);
			CheckVkResult("Failed to create debug messenger.", res);
		}

		void VulkanDebug::Free()
		{
			auto func = RCast(PFN_vkDestroyDebugUtilsMessengerEXT)(vkGetInstanceProcAddr(mInstance, "vkDestroyDebugUtilsMessengerEXT"));
			func(mInstance, mMessenger, nullptr);
		}

		VkBool32 VulkanDebug::MessageCallback(VkDebugUtilsMessageSeverityFlagBitsEXT severities,
			VkDebugUtilsMessageTypeFlagsEXT types,
			const VkDebugUtilsMessengerCallbackDataEXT* pData, void* pUserData)
		{
			bool isError = false;

			// ERROR [18:22:22.1111 / VulkanDebug:VALIDATION / ID: IDName(IDNum)] : Message
			//     Objects - 1
			//         Object[0] - Handle 123, Name "Test"
			//     Queue Labels - 1
			//         Label[0] - Test {1.0, 1.0, 1.0, 1.0}
			//     Command Buffer Labels - 1
			//         Label[0] - Test2 {1.0, 1.0, 1.0, 1.0}
			const char* prefix = "";
			
			switch(severities) {
				case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
					prefix = " VERBOSE";
					break;
				case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
					prefix = "SEVERITY";
					break;
				case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
					prefix = " WARNING";
					break;
				case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
					prefix = "   ERROR";
					isError = true;
					break;
			}

			const char* typeStr = "";
			if(types & VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT) {
				typeStr = "GENERAL";
			} else {
				if(types & VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT) {
					typeStr = "VALIDATION";

					if(types & VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT) {
						typeStr = "VALIDATION|PERFORMANCE";
					}
				}
			}

			String res = fmt::format(CUBE_T("{0} [ / VulkanDebug:{1} / ID: {2}({3})] : {4}"),
				prefix, typeStr, pData->pMessageIdName, pData->messageIdNumber, pData->pMessage);

			res += fmt::format(CUBE_T("\n\tObjects - {0}"), pData->objectCount);
			for(uint32_t i = 0; i < pData->objectCount; i++) {
				VkDebugUtilsObjectNameInfoEXT obj = pData->pObjects[i];
				res += fmt::format(CUBE_T("\n\t\tObject[{0}] - Handle {1}, Name \"{2}\""),
					GetVkObjectTypeStr(obj.objectType), obj.objectHandle, obj.pObjectName);
			}

			res += fmt::format(CUBE_T("\n\tQueue Labels - {0}"), pData->queueLabelCount);
			for(uint32_t i = 0; i < pData->queueLabelCount; i++) {
				VkDebugUtilsLabelEXT label = pData->pQueueLabels[i];
				res += fmt::format(CUBE_T("\n\t\tLabel[{0}] - {1} ({2}, {3} {4}, {5})"),
					i, label.pLabelName, label.color[0], label.color[1], label.color[2], label.color[3]);
			}

			res += fmt::format(CUBE_T("\n\tCommand Buffer Labels - {0}"), pData->cmdBufLabelCount);
			for(uint32_t i = 0; i < pData->queueLabelCount; i++) {
				VkDebugUtilsLabelEXT label = pData->pCmdBufLabels[i];
				res += fmt::format(CUBE_T("\n\t\tLabel[{0}] - {1} ({2}, {3} {4}, {5})"),
					i, label.pLabelName, label.color[0], label.color[1], label.color[2], label.color[3]);
			}

			platform::PlatformDebugUtility::PrintToConsole(res);
			if(isError) {
				platform::PlatformDebugUtility::AssertionFailed(res, __FUNCTION__, __FILE__, __LINE__);
			}

			return true;
		}

		const char* VulkanDebug::GetVkObjectTypeStr(VkObjectType type)
		{
			switch(type) {
				case VK_OBJECT_TYPE_INSTANCE:
					return "VkInstance";
					break;
				case VK_OBJECT_TYPE_PHYSICAL_DEVICE:
					return "VkPhysicalDevice";
					break;
				case VK_OBJECT_TYPE_DEVICE:
					return "VkDevice";
					break;
				case VK_OBJECT_TYPE_QUEUE:
					return "VkQueue";
					break;
				case VK_OBJECT_TYPE_SEMAPHORE:
					return "VkSemaphore";
					break;
				case VK_OBJECT_TYPE_COMMAND_BUFFER:
					return "VkCommandBuffer";
					break;
				case VK_OBJECT_TYPE_FENCE:
					return "VkFence";
					break;
				case VK_OBJECT_TYPE_DEVICE_MEMORY:
					return "VkDeviceMemory";
					break;
				case VK_OBJECT_TYPE_BUFFER:
					return "VkBuffer";
					break;
				case VK_OBJECT_TYPE_IMAGE:
					return "VkImage";
					break;
				case VK_OBJECT_TYPE_EVENT:
					return "VkEvent";
					break;
				case VK_OBJECT_TYPE_QUERY_POOL:
					return "VkQueryPool";
					break;
				case VK_OBJECT_TYPE_BUFFER_VIEW:
					return "VkBufferView";
					break;
				case VK_OBJECT_TYPE_IMAGE_VIEW:
					return "VkImageView";
					break;
				case VK_OBJECT_TYPE_SHADER_MODULE:
					return "VkShaderModule";
					break;
				case VK_OBJECT_TYPE_PIPELINE_CACHE:
					return "VkPipelineCache";
					break;
				case VK_OBJECT_TYPE_PIPELINE_LAYOUT:
					return "VkPipelineLayout";
					break;
				case VK_OBJECT_TYPE_RENDER_PASS:
					return "VkRenderPass";
					break;
				case VK_OBJECT_TYPE_PIPELINE:
					return "VkPipeline";
					break;
				case VK_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT:
					return "VkDescriptorSetLayout";
					break;
				case VK_OBJECT_TYPE_SAMPLER:
					return "VkSampler";
					break;
				case VK_OBJECT_TYPE_DESCRIPTOR_POOL:
					return "VkDescriptorPool";
					break;
				case VK_OBJECT_TYPE_DESCRIPTOR_SET:
					return "VkDescriptorSet";
					break;
				case VK_OBJECT_TYPE_FRAMEBUFFER:
					return "VkFramebuffer";
					break;
				case VK_OBJECT_TYPE_COMMAND_POOL:
					return "VkCommandPool";
					break;
				case VK_OBJECT_TYPE_SAMPLER_YCBCR_CONVERSION:
					return "VkSamplerYcbcrConversion";
					break;
				case VK_OBJECT_TYPE_DESCRIPTOR_UPDATE_TEMPLATE:
					return "VkDescriptorUpdateTemplate";
					break;
				case VK_OBJECT_TYPE_SURFACE_KHR:
					return "VkSurfaceKHR";
					break;
				case VK_OBJECT_TYPE_SWAPCHAIN_KHR:
					return "VkSwapchainKHR";
					break;
				case VK_OBJECT_TYPE_DISPLAY_KHR:
					return "VkDisplayKHR";
					break;
				case VK_OBJECT_TYPE_DISPLAY_MODE_KHR:
					return "VkDisplayModeKHR";
					break;
				case VK_OBJECT_TYPE_DEBUG_REPORT_CALLBACK_EXT:
					return "VkDebugReportCallbackEXT";
					break;
				case VK_OBJECT_TYPE_OBJECT_TABLE_NVX:
					return "VkObjectTableNVX";
					break;
				case VK_OBJECT_TYPE_INDIRECT_COMMANDS_LAYOUT_NVX:
					return "VkIndirectCommandsLayoutNVX";
					break;
				case VK_OBJECT_TYPE_DEBUG_UTILS_MESSENGER_EXT:
					return "VkDebugUtilsMessengerEXT";
					break;
				case VK_OBJECT_TYPE_VALIDATION_CACHE_EXT:
					return "VkValidationCacheEXT";
					break;
				default:
					return "Unknown";
					break;

			}
			return nullptr;
		}
	} // namespace render
} // namespace cube