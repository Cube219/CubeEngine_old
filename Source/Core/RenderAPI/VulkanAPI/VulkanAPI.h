#pragma once

#include "VulkanAPIHeader.h"

#include "BaseRenderAPI/BaseRenderAPI.h"
#include "BasePlatform/BasePlatform.h"

namespace cube
{
	namespace core
	{
		extern "C" VULKAN_API_EXPORT BaseRenderAPI* CreateAPI();

		class VULKAN_API_EXPORT VulkanAPI : public BaseRenderAPI
		{
		public:
			VulkanAPI();
			virtual ~VulkanAPI();

			void Init(SPtr<platform::BasePlatform>& platform) final override;

			SPtr<BaseRenderBuffer> CreateBuffer(BaseRenderBufferInitializer& initializer) final override;

			SPtr<BaseRenderDescriptorSetLayout> CreateDescriptorSetLayout(BaseRenderDescriptorSetInitializer& initializer) final override;

			SPtr<BaseRenderDescriptorSet> CreateDescriptorSet(SPtr<BaseRenderDescriptorSetLayout>& layout) final override;

			SPtr<BaseRenderQueue> GetQueue(QueueTypeBits types, uint32_t index) final override;

			SPtr<BaseRenderSwapchain> CreateSwapchain() final override;

			SPtr<BaseRenderRenderPass> CreateRenderPass(BaseRenderRenderPassInitializer& initializer) final override;

			SPtr<BaseRenderShader> CreateShader(BaseRenderShaderInitializer& initializer) final override;

			SPtr<BaseRenderGraphicsPipeline> CreateGraphicsPipeline(BaseRenderGraphicsPipelineInitializer& initializer) final override;

			SPtr<BaseRenderCommandBuffer> CreateCommandBuffer(bool isPrimary = true) final override;

			SPtr<BaseRenderImage> CreateImage(BaseRenderImageInitializer& initializer) final override;

			SPtr<BaseRenderSampler> CreateSampler() final override;

			SPtr<BaseRenderFence> CreateFence() final override;
#undef CreateSemaphore // Disable the macro defined in synchapi.h (WinAPI)
			SPtr<BaseRenderSemaphore> CreateSemaphore() final override;

		private:
			SPtr<VulkanInstance> mInstance;
			Vector<SPtr<VulkanPhysicalDevice>> mPhysicalDevices;
			SPtr<VulkanPhysicalDevice> mMainPhysicalDevice;
			SPtr<VulkanDevice> mDevice;

			SPtr<VulkanCommandPool> mCommandPool;

			SPtr<VulkanWindowSurface> mWindowSurface;

			SPtr<VulkanDescriptorPool> mDescriptorPool;
		};
	}
}