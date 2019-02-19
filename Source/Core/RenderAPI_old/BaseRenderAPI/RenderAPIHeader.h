#pragma once

#include "Base/BaseTypes.h"
#include "RenderUtility.h"

namespace cube
{
	namespace render
	{
		class RenderAPI;

		struct BufferInitializer;
		class Buffer;
		struct BufferInfo;
		enum class BufferTypeBits;

		class CommandBuffer;

		struct DescriptorSetInitializer;
		class DescriptorSetLayout;
		class DescriptorSet;
		enum class DescriptorType;

		class Fence;

		class Framebuffer;

		struct GraphicsPipelineInitializer;
		class GraphicsPipeline;
		enum class PipelineStageBits;

		class ImageView;
		struct ImageInitializer;
		class Image;
		enum class ImageType;
		enum class ImageViewType;
		enum class ImageUsageBits;
		enum class ImageAspectBits;
		enum class ImageLayout;

		class Sampler;

		class Queue;
		enum class QueueTypeBits;

		struct RenderPassInitializer;
		class RenderPass;
		enum class LoadOperator;
		enum class StoreOperator;
		struct Subpass;
		struct SubpassDependency;

		class Semaphore;

		struct ShaderInitializer;
		class Shader;

		class Swapchain;

		class WindowSurface;
	} // namespace render
} // namespace cube
