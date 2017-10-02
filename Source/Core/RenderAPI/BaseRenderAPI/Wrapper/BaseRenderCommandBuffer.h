#pragma once

#include "../BaseRenderAPIHeader.h"

namespace cube
{
	namespace core
	{
		class BaseRenderCommandBuffer
		{
		public:
			virtual ~BaseRenderCommandBuffer(){ }

			virtual void Reset() = 0;

			virtual void Begin() = 0;

			virtual void CopyBuffer(SPtr<BaseRenderBuffer>& source, uint64_t sourceOffset,
				SPtr<BaseRenderBuffer>& destination, uint64_t destinationOffset, uint64_t size) = 0;
			virtual void CopyBufferToImage(SPtr<BaseRenderBuffer>& buffer, uint64_t bufferOffset,
				SPtr<BaseRenderImage>& image, int imageOffsetX, int imageOffsetY, int imageOffsetZ,
				uint32_t imageWidth, uint32_t imageHeight, uint32_t imageDepth, ImageAspectBits aspectBits) = 0;

			virtual void SetRenderPass(SPtr<BaseRenderRenderPass>& renderPass, Rect2D renderArea) = 0;

			//virtual void PipelineMemoryBarrier() = 0;
			virtual void PipelineBufferMemoryBarrier(PipelineStageBits srcStage, PipelineStageBits dstStage,
				AccessBits srcAccess, AccessBits dstAccess, SPtr<BaseRenderBuffer>& buffer, uint64_t offset, uint64_t size) = 0;
			virtual void PipelineImageMemoryBarrier(PipelineStageBits srcStage, PipelineStageBits dstStage,
				AccessBits srcAccess, AccessBits dstAccess, ImageLayout oldLayout, ImageLayout newLayout,
				SPtr<BaseRenderImage>& image) = 0;

			virtual void SetViewport(uint32_t firstViewport, uint32_t viewportCount, Viewport* pViewports) = 0;
			virtual void SetScissor(uint32_t firstScissor, uint32_t scissorCount, Rect2D* pScissors) = 0;

			virtual void BindGraphicsPipeline(SPtr<BaseRenderGraphicsPipeline>& graphicsPipeline) = 0;

			virtual void BindDescriptorSets(PipelineType pipelineType, uint32_t firstSet,
				uint32_t descriptorSetNum, SPtr<BaseRenderDescriptorSet>* descriptorSets) = 0;
			virtual void BindVertexBuffers(uint32_t bufferNum, SPtr<BaseRenderBuffer>* buffers, uint64_t* bufferOffsets) = 0;
			virtual void BindIndexBuffer(SPtr<BaseRenderBuffer> buffer, uint64_t bufferOffset) = 0;

			virtual void Draw(uint32_t vertexCount, uint32_t vertexOffset, uint32_t instanceCount, uint32_t firstInstance) = 0;
			virtual void DrawIndexed(uint32_t indexCount, uint32_t indexOffset, int32_t vertexOffset, uint32_t instanceCount, uint32_t firstInstance) = 0;

			virtual void End() = 0;

			virtual void Submit(SPtr<BaseRenderQueue>& queue,
				uint32_t waitSemaphoreNum, std::pair<SPtr<BaseRenderSemaphore>, PipelineStageBits>* waitSemaphores,
				uint32_t signalSemaphoreNum, SPtr<BaseRenderSemaphore>* signalSemaphores, SPtr<BaseRenderFence> waitFence) = 0;

		protected:
			BaseRenderCommandBuffer(){ }
		};
	}
}