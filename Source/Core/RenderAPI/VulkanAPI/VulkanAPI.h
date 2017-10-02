#pragma once

#include "VulkanAPIHeader.h"

#include "BaseRenderAPI\BaseRenderAPI.h"
#include "BasePlatform\BasePlatform.h"

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

			void Init(SPtr<platform::BasePlatform>& platform) override;

			SPtr<BaseRenderBuffer> CreateBuffer(uint64_t size, BufferTypeBits types) override;

			SPtr<BaseRenderDescriptorSet> CreateDescriptorSet() override;

			SPtr<BaseRenderQueue> GetQueue(QueueTypeBits types, uint32_t index) override;

			SPtr<BaseRenderSwapchain> CreateSwapchain() override;

			SPtr<BaseRenderRenderPass> CreateRenderPass() override;

			SPtr<BaseRenderShader> CreateShader(ShaderType type, String& code, String& entryPoint) override;

			SPtr<BaseRenderGraphicsPipeline> CreateGraphicsPipeline() override;

			SPtr<BaseRenderCommandBuffer> CreateCommandBuffer() override;

			SPtr<BaseRenderImage> CreateImage(ImageType type, DataFormat format,
				uint32_t width, uint32_t height, uint32_t depth, uint32_t mipLevels, ImageUsageBits usage, bool optimal = true) override;

			SPtr<BaseRenderFence> CreateFence() override;
#undef CreateSemaphore // Disable the macro defined in synchapi.h (WinAPI)
			SPtr<BaseRenderSemaphore> CreateSemaphore() override;

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