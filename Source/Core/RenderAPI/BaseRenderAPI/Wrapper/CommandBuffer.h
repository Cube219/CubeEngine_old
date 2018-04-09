#pragma once

#include "../RenderAPIHeader.h"

namespace cube
{
	namespace render
	{
		class CommandBuffer
		{
		public:
			virtual ~CommandBuffer(){ }

			virtual void Reset() = 0;

			virtual void Begin() = 0;

			virtual void CopyBuffer(SPtr<Buffer>& source, uint64_t sourceOffset,
				SPtr<Buffer>& destination, uint64_t destinationOffset, uint64_t size) = 0;
			virtual void CopyBufferToImage(SPtr<Buffer>& buffer, uint64_t bufferOffset,
				SPtr<Image>& image, int imageOffsetX, int imageOffsetY, int imageOffsetZ,
				uint32_t imageWidth, uint32_t imageHeight, uint32_t imageDepth, ImageAspectBits aspectBits) = 0;

			virtual void SetRenderPass(SPtr<RenderPass>& renderPass, Rect2D renderArea) = 0;

			virtual void PipelineBufferMemoryBarrier(PipelineStageBits srcStage, PipelineStageBits dstStage,
				AccessBits srcAccess, AccessBits dstAccess, SPtr<Buffer>& buffer, uint64_t offset, uint64_t size) = 0;
			virtual void PipelineImageMemoryBarrier(PipelineStageBits srcStage, PipelineStageBits dstStage,
				AccessBits srcAccess, AccessBits dstAccess, ImageLayout oldLayout, ImageLayout newLayout,
				SPtr<Image>& image) = 0;

			virtual void SetViewport(uint32_t firstViewport, uint32_t viewportCount, Viewport* pViewports) = 0;
			virtual void SetScissor(uint32_t firstScissor, uint32_t scissorCount, Rect2D* pScissors) = 0;

			virtual void BindGraphicsPipeline(SPtr<GraphicsPipeline>& graphicsPipeline) = 0;

			virtual void BindDescriptorSets(PipelineType pipelineType, uint32_t firstSet,
				uint32_t descriptorSetNum, SPtr<DescriptorSet>* descriptorSets) = 0;
			virtual void BindVertexBuffers(uint32_t bufferNum, SPtr<Buffer>* buffers, uint64_t* bufferOffsets) = 0;
			virtual void BindIndexBuffer(SPtr<Buffer> buffer, uint64_t bufferOffset) = 0;

			virtual void Draw(uint32_t vertexCount, uint32_t vertexOffset, uint32_t instanceCount, uint32_t firstInstance) = 0;
			virtual void DrawIndexed(uint32_t indexCount, uint32_t indexOffset, int32_t vertexOffset, uint32_t instanceCount, uint32_t firstInstance) = 0;

			virtual void ExecuteCommands(uint32_t commandCount, SPtr<CommandBuffer>* commandBuffers) = 0;

			virtual void End() = 0;

			virtual void Submit(SPtr<Queue>& queue,
				uint32_t waitSemaphoreNum, std::pair<SPtr<Semaphore>, PipelineStageBits>* waitSemaphores,
				uint32_t signalSemaphoreNum, SPtr<Semaphore>* signalSemaphores, SPtr<Fence> waitFence) = 0;

		protected:
			CommandBuffer(){ }
		};
	} // namespace render
} // namespace cube
