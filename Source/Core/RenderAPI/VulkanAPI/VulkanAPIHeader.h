#pragma once
#pragma warning(disable:4251)
#pragma warning(disable:4275)

#include "Base/BaseTypes.h"

#include <vulkan/vulkan.h>
#include "VulkanUtility.h"

namespace cube
{
	namespace render
	{
		class VulkanAPI;

		class VulkanBuffer;
		
		class VulkanCommandBuffer;
		
		class VulkanCommandPool;
		
		class VulkanDescriptorPool;
		
		class VulkanDescriptorSetLayout;
		class VulkanDescriptorSet;
		
		class VulkanDeviceInitializer;
		class VulkanDevice;
		
		class VulkanFence;
		
		struct VulkanFramebufferInitializer;
		class VulkanFramebuffer;
		
		class VulkanGraphicsPipeline;
		
		class VulkanImageView;
		
		class VulkanImage;

		class VulkanSampler;
		
		struct VulkanInstanceInitializer;
		class VulkanInstance;
		
		enum class VulkanPhysicalDeviceFeature;
		class VulkanPhysicalDeviceFeatures
		{
		public:
			VulkanPhysicalDeviceFeatures();
			VulkanPhysicalDeviceFeatures(VkPhysicalDeviceFeatures features);

			void SetFeature(VulkanPhysicalDeviceFeature feature, VkBool32 on);

			operator VkPhysicalDeviceFeatures()
			{
				return vkFeatures;
			}

			VulkanPhysicalDeviceFeatures operator&(const VulkanPhysicalDeviceFeatures& other);
			VulkanPhysicalDeviceFeatures operator|(const VulkanPhysicalDeviceFeatures& other);

		private:
			VkPhysicalDeviceFeatures vkFeatures;
		};
		class VulkanPhysicalDevice;
		
		class VulkanQueue;
		struct VulkanQueueFamily
		{
			VkQueueFamilyProperties mProperties;
			uint32_t mIndex;
		};
		
		class VulkanSubpass;
		class VulkanRenderPass;
		
		class VulkanSemaphore;
		
		class VulkanShader;
		
		class VulkanSwapchain;
		struct VulkanSwapchainImage
		{
			SPtr<VulkanImage> colorImage;
			SPtr<VulkanImageView> colorImageView;
		};
		
		class VulkanWindowSurface;
	} // namespace render
} // namespace cube

#ifdef VULKAN_API_EXPORTS
#define VULKAN_API_EXPORT __declspec(dllexport) 
#else // VULKAN_API_EXPORTS
#define VULKAN_API_EXPORT __declspec(dllimport) 
#endif // VULKAN_API_EXPORTS
