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

		private:
			static VkBool32 MessageCallback(VkDebugUtilsMessageSeverityFlagBitsEXT severities,
				VkDebugUtilsMessageTypeFlagsEXT types,
				const VkDebugUtilsMessengerCallbackDataEXT* pData,
				void* pUserData);

			static const char* GetVkObjectTypeStr(VkObjectType type);

			static VkInstance mInstance;
			static VkDebugUtilsMessengerEXT mMessenger;
		};
	} // namespace render
} // namespace cube
