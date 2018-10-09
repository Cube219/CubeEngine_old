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

			static void SetObjectName(VkDevice device, VkInstance instance, const char* name);
			static void SetObjectName(VkDevice device, VkPhysicalDevice physicalDevice, const char* name);
			static void SetObjectName(VkDevice device, const char* name);
			static void SetObjectName(VkDevice device, VkQueue queue, const char* name);
			static void SetObjectName(VkDevice device, VkSemaphore semaphore, const char* name);
			static void SetObjectName(VkDevice device, VkCommandBuffer commandBuf, const char* name);
			static void SetObjectName(VkDevice device, VkFence fence, const char* name);
			static void SetObjectName(VkDevice device, VkDeviceMemory deviceMemory, const char* name);
			static void SetObjectName(VkDevice device, VkBuffer buffer, const char* name);
			static void SetObjectName(VkDevice device, VkImage image, const char* name);
			static void SetObjectName(VkDevice device, VkEvent event, const char* name);
			static void SetObjectName(VkDevice device, VkQueryPool queryPool, const char* name);
			static void SetObjectName(VkDevice device, VkBufferView bufferView, const char* name);
			static void SetObjectName(VkDevice device, VkImageView imageView, const char* name);
			static void SetObjectName(VkDevice device, VkShaderModule shaderModule, const char* name);
			static void SetObjectName(VkDevice device, VkPipelineCache pipelineCache, const char* name);
			static void SetObjectName(VkDevice device, VkPipelineLayout pipelineLayout, const char* name);
			static void SetObjectName(VkDevice device, VkRenderPass renderPass, const char* name);
			static void SetObjectName(VkDevice device, VkPipeline pipeline, const char* name);
			static void SetObjectName(VkDevice device, VkDescriptorSetLayout descriptorSetLayout, const char* name);
			static void SetObjectName(VkDevice device, VkSampler sampler, const char* name);
			static void SetObjectName(VkDevice device, VkDescriptorPool descriptorPool, const char* name);
			static void SetObjectName(VkDevice device, VkDescriptorSet descriptorSet, const char* name);
			static void SetObjectName(VkDevice device, VkFramebuffer framebuffer, const char* name);
			static void SetObjectName(VkDevice device, VkCommandPool commandPool, const char* name);
			static void SetObjectName(VkDevice device, VkSamplerYcbcrConversion samplerYcbcrConversion, const char* name);
			static void SetObjectName(VkDevice device, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const char* name);
			static void SetObjectName(VkDevice device, VkSurfaceKHR surface, const char* name);
			static void SetObjectName(VkDevice device, VkSwapchainKHR swapchain, const char* name);
			static void SetObjectName(VkDevice device, VkDisplayKHR display, const char* name);
			static void SetObjectName(VkDevice device, VkDisplayModeKHR displayMode, const char* name);
			static void SetObjectName(VkDevice device, VkDebugReportCallbackEXT debugReportCallback, const char* name);
			static void SetObjectName(VkDevice device, VkObjectTableNVX objectTable, const char* name);
			static void SetObjectName(VkDevice device, VkIndirectCommandsLayoutNVX indirectCommandsLayout, const char* name);
			static void SetObjectName(VkDevice device, VkDebugUtilsMessengerEXT debugUtilsMessenger, const char* name);
			static void SetObjectName(VkDevice device, VkValidationCacheEXT validationCache, const char* name);

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

			static VkInstance mInstance;
			static VkDebugUtilsMessengerEXT mMessenger;

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
		};
	} // namespace render
} // namespace cube
