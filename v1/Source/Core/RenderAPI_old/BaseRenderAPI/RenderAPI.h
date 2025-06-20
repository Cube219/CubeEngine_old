#pragma once

#include "RenderAPIHeader.h"

namespace cube
{
	namespace render
	{
		class RenderAPI
		{
		public:
			RenderAPI() {};
			virtual ~RenderAPI() {}

			virtual void Init() = 0;

			virtual SPtr<Buffer> CreateBuffer(BufferInitializer& initializer) = 0;

			virtual SPtr<DescriptorSetLayout> CreateDescriptorSetLayout(DescriptorSetInitializer& initializer) = 0;

			virtual SPtr<DescriptorSet> CreateDescriptorSet(SPtr<DescriptorSetLayout>& layout) = 0;

			virtual SPtr<Queue> GetQueue(QueueTypeBits types, uint32_t index) = 0;

			virtual SPtr<Swapchain> CreateSwapchain() = 0;

			virtual SPtr<RenderPass> CreateRenderPass(RenderPassInitializer& initializer) = 0;

			virtual SPtr<Shader> CreateShader(ShaderInitializer& initializer) = 0;

			virtual SPtr<GraphicsPipeline> CreateGraphicsPipeline(GraphicsPipelineInitializer& initializer) = 0;

			virtual SPtr<CommandBuffer> CreateCommandBuffer(bool isPrimary = true) = 0;

			virtual SPtr<Image> CreateImage(ImageInitializer& initializer) = 0;

			virtual SPtr<Sampler> CreateSampler() = 0;

			virtual SPtr<Fence> CreateFence() = 0;
#undef CreateSemaphore // Disable the macro defined in synchapi.h (WinAPI)
			virtual SPtr<Semaphore> CreateSemaphore() = 0;
		};
	} // namespace render
} // namespace cube
