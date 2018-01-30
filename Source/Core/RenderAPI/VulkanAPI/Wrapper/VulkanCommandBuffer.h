#pragma once

#include "../VulkanAPIHeader.h"

#include "BaseRenderAPI/Wrapper/BaseRenderCommandBuffer.h"

namespace cube
{
	namespace core
	{
		class VULKAN_API_EXPORT VulkanCommandBuffer : public BaseRenderCommandBuffer
		{
			friend VulkanCommandPool;

		public:
			virtual ~VulkanCommandBuffer();

			VkCommandBuffer GetHandle() const { return mCommandBuffer; }

			void Reset() final override;

			void Begin() final override;

			void CopyBuffer(SPtr<BaseRenderBuffer>& source, uint64_t sourceOffset,
				SPtr<BaseRenderBuffer>& destination, uint64_t destinationOffset, uint64_t size) final override;
			void CopyBufferToImage(SPtr<BaseRenderBuffer>& buffer, uint64_t bufferOffset,
				SPtr<BaseRenderImage>& image, int imageOffsetX, int imageOffsetY, int imageOffsetZ,
				uint32_t imageWidth, uint32_t imageHeight, uint32_t imageDepth, ImageAspectBits aspectBits) final override;

			void SetRenderPass(SPtr<BaseRenderRenderPass>& renderPass, Rect2D renderArea) final override;

			void PipelineBufferMemoryBarrier(PipelineStageBits srcStage, PipelineStageBits dstStage,
				AccessBits srcAccess, AccessBits dstAccess, SPtr<BaseRenderBuffer>& buffer, uint64_t offset, uint64_t size) final override;
			void PipelineImageMemoryBarrier(PipelineStageBits srcStage, PipelineStageBits dstStage,
				AccessBits srcAccess, AccessBits dstAccess, ImageLayout oldLayout, ImageLayout newLayout,
				SPtr<BaseRenderImage>& image) final override;

			void SetViewport(uint32_t firstViewport, uint32_t viewportCount, Viewport* pViewports) final override;
			void SetScissor(uint32_t firstScissor, uint32_t scissorCount, Rect2D* pScissors) final override;

			void BindGraphicsPipeline(SPtr<BaseRenderGraphicsPipeline>& graphicsPipeline) final override;

			void BindDescriptorSets(PipelineType pipelineType, uint32_t firstSet,
				uint32_t descriptorSetNum, SPtr<BaseRenderDescriptorSet>* descriptorSets) final override;
			void BindVertexBuffers(uint32_t bufferNum, SPtr<BaseRenderBuffer>* buffers, uint64_t* bufferOffsets) final override;
			void BindIndexBuffer(SPtr<BaseRenderBuffer> buffer, uint64_t bufferOffset) final override;

			void Draw(uint32_t vertexCount, uint32_t vertexOffset, uint32_t instanceCount, uint32_t firstInstance) final override;
			void DrawIndexed(uint32_t indexCount, uint32_t indexOffset, int32_t vertexOffset, uint32_t instanceCount, uint32_t firstInstance) final override;

			void ExecuteCommands(uint32_t commandCount, SPtr<BaseRenderCommandBuffer>* commandBuffers) final override;

			void End() final override;
			void Submit(SPtr<BaseRenderQueue>& queue,
				uint32_t waitSemaphoreNum, std::pair<SPtr<BaseRenderSemaphore>, PipelineStageBits>* waitSemaphores,
				uint32_t signalSemaphoreNum, SPtr<BaseRenderSemaphore>* signalSemaphores, SPtr<BaseRenderFence> waitFence) final override;

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
	}
}