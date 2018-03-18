#pragma once

#include "BaseRenderAPIHeader.h"

namespace cube
{
	namespace core
	{
		class BaseRenderAPI
		{
		public:
			BaseRenderAPI() {};
			virtual ~BaseRenderAPI() {}

			virtual void Init() = 0;

			virtual SPtr<BaseRenderBuffer> CreateBuffer(BaseRenderBufferInitializer& initializer) = 0;

			virtual SPtr<BaseRenderDescriptorSetLayout> CreateDescriptorSetLayout(BaseRenderDescriptorSetInitializer& initializer) = 0;

			virtual SPtr<BaseRenderDescriptorSet> CreateDescriptorSet(SPtr<BaseRenderDescriptorSetLayout>& layout) = 0;

			virtual SPtr<BaseRenderQueue> GetQueue(QueueTypeBits types, uint32_t index) = 0;

			virtual SPtr<BaseRenderSwapchain> CreateSwapchain() = 0;

			virtual SPtr<BaseRenderRenderPass> CreateRenderPass(BaseRenderRenderPassInitializer& initializer) = 0;

			virtual SPtr<BaseRenderShader> CreateShader(BaseRenderShaderInitializer& initializer) = 0;

			virtual SPtr<BaseRenderGraphicsPipeline> CreateGraphicsPipeline(BaseRenderGraphicsPipelineInitializer& initializer) = 0;

			virtual SPtr<BaseRenderCommandBuffer> CreateCommandBuffer(bool isPrimary = true) = 0;

			virtual SPtr<BaseRenderImage> CreateImage(BaseRenderImageInitializer& initializer) = 0;

			virtual SPtr<BaseRenderSampler> CreateSampler() = 0;

			virtual SPtr<BaseRenderFence> CreateFence() = 0;
#undef CreateSemaphore // Disable the macro defined in synchapi.h (WinAPI)
			virtual SPtr<BaseRenderSemaphore> CreateSemaphore() = 0;
		};
	}
}