#pragma once

#include <vulkan/vulkan.h>

namespace cube
{
	namespace render
	{
		class VulkanDebug
		{
		public:
			VulkanDebug() = delete;
			~VulkanDebug() = delete;

			static void Setup(VkInstance instance);
			static void Free();

			static void SetObjectName(VkDevice device, VkInstance instance, const char* name)
			{
				SetObjectNameImpl(device, VK_OBJECT_TYPE_INSTANCE, (uint64_t)instance, name);
			}
			static void SetObjectName(VkDevice device, VkPhysicalDevice physicalDevice, const char* name)
			{
				SetObjectNameImpl(device, VK_OBJECT_TYPE_PHYSICAL_DEVICE, (uint64_t)physicalDevice, name);
			}
			static void SetObjectName(VkDevice device, const char* name)
			{
				SetObjectNameImpl(device, VK_OBJECT_TYPE_DEVICE, (uint64_t)device, name);
			}
			static void SetObjectName(VkDevice device, VkQueue queue, const char* name)
			{
				SetObjectNameImpl(device, VK_OBJECT_TYPE_QUEUE, (uint64_t)queue, name);
			}
			static void SetObjectName(VkDevice device, VkSemaphore semaphore, const char* name)
			{
				SetObjectNameImpl(device, VK_OBJECT_TYPE_SEMAPHORE, (uint64_t)semaphore, name);
			}
			static void SetObjectName(VkDevice device, VkCommandBuffer commandBuf, const char* name)
			{
				SetObjectNameImpl(device, VK_OBJECT_TYPE_COMMAND_BUFFER, (uint64_t)commandBuf, name);
			}
			static void SetObjectName(VkDevice device, VkFence fence, const char* name)
			{
				SetObjectNameImpl(device, VK_OBJECT_TYPE_FENCE, (uint64_t)fence, name);
			}
			static void SetObjectName(VkDevice device, VkDeviceMemory deviceMemory, const char* name)
			{
				SetObjectNameImpl(device, VK_OBJECT_TYPE_DEVICE_MEMORY, (uint64_t)deviceMemory, name);
			}
			static void SetObjectName(VkDevice device, VkBuffer buffer, const char* name)
			{
				SetObjectNameImpl(device, VK_OBJECT_TYPE_BUFFER, (uint64_t)buffer, name);
			}
			static void SetObjectName(VkDevice device, VkImage image, const char* name)
			{
				SetObjectNameImpl(device, VK_OBJECT_TYPE_IMAGE, (uint64_t)image, name);
			}
			static void SetObjectName(VkDevice device, VkEvent event, const char* name)
			{
				SetObjectNameImpl(device, VK_OBJECT_TYPE_EVENT, (uint64_t)event, name);
			}
			static void SetObjectName(VkDevice device, VkQueryPool queryPool, const char* name)
			{
				SetObjectNameImpl(device, VK_OBJECT_TYPE_QUERY_POOL, (uint64_t)queryPool, name);
			}
			static void SetObjectName(VkDevice device, VkBufferView bufferView, const char* name)
			{
				SetObjectNameImpl(device, VK_OBJECT_TYPE_BUFFER_VIEW, (uint64_t)bufferView, name);
			}
			static void SetObjectName(VkDevice device, VkImageView imageView, const char* name)
			{
				SetObjectNameImpl(device, VK_OBJECT_TYPE_IMAGE_VIEW, (uint64_t)imageView, name);
			}
			static void SetObjectName(VkDevice device, VkShaderModule shaderModule, const char* name)
			{
				SetObjectNameImpl(device, VK_OBJECT_TYPE_SHADER_MODULE, (uint64_t)shaderModule, name);
			}
			static void SetObjectName(VkDevice device, VkPipelineCache pipelineCache, const char* name)
			{
				SetObjectNameImpl(device, VK_OBJECT_TYPE_PIPELINE_CACHE, (uint64_t)pipelineCache, name);
			}
			static void SetObjectName(VkDevice device, VkPipelineLayout pipelineLayout, const char* name)
			{
				SetObjectNameImpl(device, VK_OBJECT_TYPE_PIPELINE_LAYOUT, (uint64_t)pipelineLayout, name);
			}
			static void SetObjectName(VkDevice device, VkRenderPass renderPass, const char* name)
			{
				SetObjectNameImpl(device, VK_OBJECT_TYPE_RENDER_PASS, (uint64_t)renderPass, name);
			}
			static void SetObjectName(VkDevice device, VkPipeline pipeline, const char* name)
			{
				SetObjectNameImpl(device, VK_OBJECT_TYPE_PIPELINE, (uint64_t)pipeline, name);
			}
			static void SetObjectName(VkDevice device, VkDescriptorSetLayout descriptorSetLayout, const char* name)
			{
				SetObjectNameImpl(device, VK_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT, (uint64_t)descriptorSetLayout, name);
			}
			static void SetObjectName(VkDevice device, VkSampler sampler, const char* name)
			{
				SetObjectNameImpl(device, VK_OBJECT_TYPE_SAMPLER, (uint64_t)sampler, name);
			}
			static void SetObjectName(VkDevice device, VkDescriptorPool descriptorPool, const char* name)
			{
				SetObjectNameImpl(device, VK_OBJECT_TYPE_DESCRIPTOR_POOL, (uint64_t)descriptorPool, name);
			}
			static void SetObjectName(VkDevice device, VkDescriptorSet descriptorSet, const char* name)
			{
				SetObjectNameImpl(device, VK_OBJECT_TYPE_DESCRIPTOR_SET, (uint64_t)descriptorSet, name);
			}
			static void SetObjectName(VkDevice device, VkFramebuffer framebuffer, const char* name)
			{
				SetObjectNameImpl(device, VK_OBJECT_TYPE_FRAMEBUFFER, (uint64_t)framebuffer, name);
			}
			static void SetObjectName(VkDevice device, VkCommandPool commandPool, const char* name)
			{
				SetObjectNameImpl(device, VK_OBJECT_TYPE_COMMAND_POOL, (uint64_t)commandPool, name);
			}
			static void SetObjectName(VkDevice device, VkSamplerYcbcrConversion samplerYcbcrConversion, const char* name)
			{
				SetObjectNameImpl(device, VK_OBJECT_TYPE_SAMPLER_YCBCR_CONVERSION, (uint64_t)samplerYcbcrConversion, name);
			}
			static void SetObjectName(VkDevice device, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const char* name)
			{
				SetObjectNameImpl(device, VK_OBJECT_TYPE_DESCRIPTOR_UPDATE_TEMPLATE, (uint64_t)descriptorUpdateTemplate, name);
			}
			static void SetObjectName(VkDevice device, VkSurfaceKHR surface, const char* name)
			{
				SetObjectNameImpl(device, VK_OBJECT_TYPE_SURFACE_KHR, (uint64_t)surface, name);
			}
			static void SetObjectName(VkDevice device, VkSwapchainKHR swapchain, const char* name)
			{
				SetObjectNameImpl(device, VK_OBJECT_TYPE_SWAPCHAIN_KHR, (uint64_t)swapchain, name);
			}
			static void SetObjectName(VkDevice device, VkDisplayKHR display, const char* name)
			{
				SetObjectNameImpl(device, VK_OBJECT_TYPE_DISPLAY_KHR, (uint64_t)display, name);
			}
			static void SetObjectName(VkDevice device, VkDisplayModeKHR displayMode, const char* name)
			{
				SetObjectNameImpl(device, VK_OBJECT_TYPE_DISPLAY_MODE_KHR, (uint64_t)displayMode, name);
			}
			static void SetObjectName(VkDevice device, VkDebugReportCallbackEXT debugReportCallback, const char* name)
			{
				SetObjectNameImpl(device, VK_OBJECT_TYPE_DEBUG_REPORT_CALLBACK_EXT, (uint64_t)debugReportCallback, name);
			}
			static void SetObjectName(VkDevice device, VkObjectTableNVX objectTable, const char* name)
			{
				SetObjectNameImpl(device, VK_OBJECT_TYPE_OBJECT_TABLE_NVX, (uint64_t)objectTable, name);
			}
			static void SetObjectName(VkDevice device, VkIndirectCommandsLayoutNVX indirectCommandsLayout, const char* name)
			{
				SetObjectNameImpl(device, VK_OBJECT_TYPE_INDIRECT_COMMANDS_LAYOUT_NVX, (uint64_t)indirectCommandsLayout, name);
			}
			static void SetObjectName(VkDevice device, VkDebugUtilsMessengerEXT debugUtilsMessenger, const char* name)
			{
				SetObjectNameImpl(device, VK_OBJECT_TYPE_DEBUG_UTILS_MESSENGER_EXT, (uint64_t)debugUtilsMessenger, name);
			}
			static void SetObjectName(VkDevice device, VkValidationCacheEXT validationCache, const char* name)
			{
				SetObjectNameImpl(device, VK_OBJECT_TYPE_VALIDATION_CACHE_EXT, (uint64_t)validationCache, name);
			}

			static void BeginQueueLabel(VkQueue queue, const char* name, float color[4]);
			static void EndQueueLabel(VkQueue queue);
			static void InsertQueueLabel(VkQueue queue, const char* name, float color[4]);

			static void BeginCommandBufferLabel(VkCommandBuffer commandBuf, const char* name, float color[4]);
			static void EndCommandBufferLabel(VkCommandBuffer commandBuf);
			static void InsertCommandBufferLabel(VkCommandBuffer commandBuf, const char* name, float color[4]);
		private:
			static VkBool32 MessageCallback(VkDebugUtilsMessageSeverityFlagBitsEXT severities,
				VkDebugUtilsMessageTypeFlagsEXT types,
				const VkDebugUtilsMessengerCallbackDataEXT* pData,
				void* pUserData);

			static void SetObjectNameImpl(VkDevice device, VkObjectType type, uint64_t handle, const char* name);
			// static void SetObjectTagImpl(VkDevice device, VkObjectType type, uint64_t handle);

			static VkDebugUtilsLabelEXT CreateLabel(const char* name, float color[4]);

			static const char* GetVkObjectTypeStr(VkObjectType type);

			static VkInstance instance;
			static VkDebugUtilsMessengerEXT messenger;

			static PFN_vkCreateDebugUtilsMessengerEXT createDebugUtilsMessengerEXT;
			static PFN_vkDestroyDebugUtilsMessengerEXT destroyDebugUtilsMessengerEXT;
			static PFN_vkSetDebugUtilsObjectNameEXT setDebugUtilsObjectNameEXT;
			static PFN_vkSetDebugUtilsObjectTagEXT setDebugUtilsObjectTagEXT;
			static PFN_vkQueueBeginDebugUtilsLabelEXT queueBeginDebugUtilsLabelEXT;
			static PFN_vkQueueEndDebugUtilsLabelEXT queueEndDebugUtilsLabelEXT;
			static PFN_vkQueueInsertDebugUtilsLabelEXT queueInsertDebugUtilsLabelEXT;
			static PFN_vkCmdBeginDebugUtilsLabelEXT cmdBeginDebugUtilsLabelEXT;
			static PFN_vkCmdEndDebugUtilsLabelEXT cmdEndDebugUtilsLabelEXT;
			static PFN_vkCmdInsertDebugUtilsLabelEXT cmdInsertDebugUtilsLabelEXT;

			static bool isEnabled;
		};
	} // namespace render
} // namespace cube
