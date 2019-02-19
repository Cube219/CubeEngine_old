#pragma once

#include "../VulkanAPIHeader.h"

#include "BaseRenderAPI/Wrapper/CommandBuffer.h"

namespace cube
{
	namespace render
	{
		class VULKAN_API_EXPORT VulkanCommandBuffer : public CommandBuffer
		{
			friend VulkanCommandPool;

		public:
			virtual ~VulkanCommandBuffer();

			VkCommandBuffer GetHandle() const { return mCommandBuffer; }

			void Reset() final override;

			void Begin() final override;

			void CopyBuffer(SPtr<Buffer>& source, uint64_t sourceOffset,
				SPtr<Buffer>& destination, uint64_t destinationOffset, uint64_t size) final override;
			void CopyBufferToImage(SPtr<Buffer>& buffer, uint64_t bufferOffset,
				SPtr<Image>& image, int imageOffsetX, int imageOffsetY, int imageOffsetZ,
				uint32_t imageWidth, uint32_t imageHeight, uint32_t imageDepth, ImageAspectBits aspectBits) final override;

			void SetRenderPass(SPtr<RenderPass>& renderPass, Rect2D renderArea) final override;

			void PipelineBufferMemoryBarrier(PipelineStageBits srcStage, PipelineStageBits dstStage,
				AccessBits srcAccess, AccessBits dstAccess, SPtr<Buffer>& buffer, uint64_t offset, uint64_t size) final override;
			void PipelineImageMemoryBarrier(PipelineStageBits srcStage, PipelineStageBits dstStage,
				AccessBits srcAccess, AccessBits dstAccess, ImageLayout oldLayout, ImageLayout newLayout,
				SPtr<Image>& image) final override;

			void SetViewport(uint32_t firstViewport, uint32_t viewportCount, Viewport* pViewports) final override;
			void SetScissor(uint32_t firstScissor, uint32_t scissorCount, Rect2D* pScissors) final override;

			void BindGraphicsPipeline(SPtr<GraphicsPipeline>& graphicsPipeline) final override;

			void BindDescriptorSets(PipelineType pipelineType, uint32_t firstSet,
				uint32_t descriptorSetNum, SPtr<DescriptorSet>* descriptorSets) final override;
			void BindVertexBuffers(uint32_t bufferNum, SPtr<Buffer>* buffers, uint64_t* bufferOffsets) final override;
			void BindIndexBuffer(SPtr<Buffer> buffer, uint64_t bufferOffset) final override;

			void Draw(uint32_t vertexCount, uint32_t vertexOffset, uint32_t instanceCount, uint32_t firstInstance) final override;
			void DrawIndexed(uint32_t indexCount, uint32_t indexOffset, int32_t vertexOffset, uint32_t instanceCount, uint32_t firstInstance) final override;

			void ExecuteCommands(uint32_t commandCount, SPtr<CommandBuffer>* commandBuffers) final override;

			void End() final override;
			void Submit(SPtr<Queue>& queue,
				uint32_t waitSemaphoreNum, std::pair<SPtr<Semaphore>, PipelineStageBits>* waitSemaphores,
				uint32_t signalSemaphoreNum, SPtr<Semaphore>* signalSemaphores, SPtr<Fence> waitFence) final override;

		private:
			VulkanCommandBuffer(){ }
			VulkanCommandBuffer(SPtr<VulkanDevice>& device, SPtr<VulkanCommandPool>& commandPool, VkCommandBuffer commandBuffer);

			VkPipelineBindPoint GetVkPipelineBindPoint(PipelineType pipelineType);

			VkCommandBuffer mCommandBuffer;

			bool mIsPrimary;

			bool mIsRenderPassStarted;

			SPtr<VulkanGraphicsPipeline> mGraphicsPipeline;

			SPtr<VulkanDevice> mDevice_ref;
			SPtr<VulkanCommandPool> mCommandPool_ref;
		};

		class VulkanCommandPool : public std::enable_shared_from_this<VulkanCommandPool>
		{
		public:
			VulkanCommandPool(SPtr<VulkanDevice>& device, VulkanQueueFamily designatedQueueFamily);
			~VulkanCommandPool();

			SPtr<VulkanCommandBuffer> AllocateCommandBuffer(VkCommandBufferLevel level);

			VkCommandPool GetHandle() const{ return mCommandPool; }

		private:
			VkCommandPool mCommandPool;

			VulkanQueueFamily mDesignatedQueueFamily;

			SPtr<VulkanDevice> mDevice_ref;
		};
	} // namespace render
} // namespace cube
