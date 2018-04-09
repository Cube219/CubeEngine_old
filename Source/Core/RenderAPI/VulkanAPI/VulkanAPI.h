#pragma once

#include "VulkanAPIHeader.h"

#include "BaseRenderAPI/RenderAPI.h"

namespace cube
{
	namespace render
	{
		extern "C" VULKAN_API_EXPORT RenderAPI* CreateAPI();

		class VULKAN_API_EXPORT VulkanAPI : public RenderAPI
		{
		public:
			VulkanAPI();
			virtual ~VulkanAPI();

			void Init() final override;

			SPtr<Buffer> CreateBuffer(BufferInitializer& initializer) final override;

			SPtr<DescriptorSetLayout> CreateDescriptorSetLayout(DescriptorSetInitializer& initializer) final override;

			SPtr<DescriptorSet> CreateDescriptorSet(SPtr<DescriptorSetLayout>& layout) final override;

			SPtr<Queue> GetQueue(QueueTypeBits types, uint32_t index) final override;

			SPtr<Swapchain> CreateSwapchain() final override;

			SPtr<RenderPass> CreateRenderPass(RenderPassInitializer& initializer) final override;

			SPtr<Shader> CreateShader(ShaderInitializer& initializer) final override;

			SPtr<GraphicsPipeline> CreateGraphicsPipeline(GraphicsPipelineInitializer& initializer) final override;

			SPtr<CommandBuffer> CreateCommandBuffer(bool isPrimary = true) final override;

			SPtr<Image> CreateImage(ImageInitializer& initializer) final override;

			SPtr<Sampler> CreateSampler() final override;

			SPtr<Fence> CreateFence() final override;
#undef CreateSemaphore // Disable the macro defined in synchapi.h (WinAPI)
			SPtr<Semaphore> CreateSemaphore() final override;

		private:
			SPtr<VulkanInstance> mInstance;
			Vector<SPtr<VulkanPhysicalDevice>> mPhysicalDevices;
			SPtr<VulkanPhysicalDevice> mMainPhysicalDevice;
			SPtr<VulkanDevice> mDevice;

			SPtr<VulkanCommandPool> mCommandPool;

			SPtr<VulkanWindowSurface> mWindowSurface;

			SPtr<VulkanDescriptorPool> mDescriptorPool;
		};
	} // namespace render
} // namespace cube
