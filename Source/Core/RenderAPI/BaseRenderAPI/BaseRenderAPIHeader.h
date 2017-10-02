#pragma once

#include "Base\BaseTypes.h"
#include "BaseRenderUtility.h"

namespace cube
{
	namespace core
	{
		class BaseRenderAPI;

		class BaseRenderBuffer;
		struct BaseRenderBufferInfo
		{
			WPtr<BaseRenderBuffer> buffer;
			uint64_t offset;
			uint64_t range;
		};
		enum class BufferTypeBits;

		class BaseRenderCommandBuffer;

		class BaseRenderDescriptorSet;
		enum class DescriptorType;

		class BaseRenderFence;

		class BaseRenderFramebuffer;

		class BaseRenderGraphicsPipeline;
		enum class PipelineStageBits;

		class BaseRenderImageView;
		class BaseRenderImage;
		enum class ImageType;
		enum class ImageViewType;
		enum class ImageUsageBits;
		enum class ImageAspectBits;
		enum class ImageLayout;

		class BaseRenderSampler;

		class BaseRenderQueue;
		enum class QueueTypeBits;

		class BaseRenderRenderPass;
		enum class LoadOperator;
		enum class StoreOperator;
		struct BaseRenderSubpass
		{
			struct AttachmentRef
			{
				uint32_t index;
				ImageLayout layout;
			};
			Vector<AttachmentRef> mInputs;
			Vector<AttachmentRef> mColors;
			AttachmentRef mDepthStencil;
		};

		class BaseRenderSemaphore;

		class BaseRenderShader;

		class BaseRenderSwapchain;

		class BaseRenderWindowSurface;
	}
}