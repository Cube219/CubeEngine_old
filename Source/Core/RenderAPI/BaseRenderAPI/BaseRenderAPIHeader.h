#pragma once

#include "Base/BaseTypes.h"
#include "BaseRenderUtility.h"

namespace cube
{
	namespace core
	{
		class BaseRenderAPI;

		struct BaseRenderBufferInitializer;
		class BaseRenderBuffer;
		struct BaseRenderBufferInfo;
		enum class BufferTypeBits;

		class BaseRenderCommandBuffer;

		struct BaseRenderDescriptorSetInitializer;
		class BaseRenderDescriptorSetLayout;
		class BaseRenderDescriptorSet;
		enum class DescriptorType;

		class BaseRenderFence;

		class BaseRenderFramebuffer;

		struct BaseRenderGraphicsPipelineInitializer;
		class BaseRenderGraphicsPipeline;
		enum class PipelineStageBits;

		class BaseRenderImageView;
		struct BaseRenderImageInitializer;
		class BaseRenderImage;
		enum class ImageType;
		enum class ImageViewType;
		enum class ImageUsageBits;
		enum class ImageAspectBits;
		enum class ImageLayout;

		class BaseRenderSampler;

		class BaseRenderQueue;
		enum class QueueTypeBits;

		struct BaseRenderRenderPassInitializer;
		class BaseRenderRenderPass;
		enum class LoadOperator;
		enum class StoreOperator;
		struct BaseRenderSubpass;
		struct BaseRenderSubpassDependency;

		class BaseRenderSemaphore;

		struct BaseRenderShaderInitializer;
		class BaseRenderShader;

		class BaseRenderSwapchain;

		class BaseRenderWindowSurface;
	}
}