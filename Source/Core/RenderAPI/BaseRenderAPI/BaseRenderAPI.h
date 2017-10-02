#pragma once

#include "BaseRenderAPIHeader.h"
#include "BasePlatform/BasePlatform.h"

namespace cube
{
	namespace core
	{
		class BaseRenderAPI
		{
		public:
			BaseRenderAPI() {};
			virtual ~BaseRenderAPI() {}

			virtual void Init(SPtr<platform::BasePlatform>& platform) = 0;

			virtual SPtr<BaseRenderBuffer> CreateBuffer(uint64_t size, BufferTypeBits types) = 0;

			virtual SPtr<BaseRenderDescriptorSet> CreateDescriptorSet() = 0;

			virtual SPtr<BaseRenderQueue> GetQueue(QueueTypeBits types, uint32_t index) = 0;

			virtual SPtr<BaseRenderSwapchain> CreateSwapchain() = 0;

			virtual SPtr<BaseRenderRenderPass> CreateRenderPass() = 0;

			virtual SPtr<BaseRenderShader> CreateShader(ShaderType type, String& code, String& entryPoint) = 0;

			virtual SPtr<BaseRenderGraphicsPipeline> CreateGraphicsPipeline() = 0;

			virtual SPtr<BaseRenderCommandBuffer> CreateCommandBuffer() = 0;

			virtual SPtr<BaseRenderImage> CreateImage(ImageType type, DataFormat format,
				uint32_t width, uint32_t height, uint32_t depth, uint32_t mipLevels, ImageUsageBits usage, bool optimal = true) = 0;

			virtual SPtr<BaseRenderSampler> CreateSampler() = 0;

			virtual SPtr<BaseRenderFence> CreateFence() = 0;
#undef CreateSemaphore // Disable the macro defined in synchapi.h (WinAPI)
			virtual SPtr<BaseRenderSemaphore> CreateSemaphore() = 0;
		};
	}
}