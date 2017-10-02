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

			operator VkCommandBuffer() const
			{
				return mCommandBuffer;
			}

			void Reset() override;

			void Begin() override;

			void CopyBuffer(SPtr<BaseRenderBuffer>& source, uint64_t sourceOffset,
				SPtr<BaseRenderBuffer>& destination, uint64_t destinationOffset, uint64_t size) override;
			void CopyBufferToImage(SPtr<BaseRenderBuffer>& buffer, uint64_t bufferOffset,
				SPtr<BaseRenderImage>& image, int imageOffsetX, int imageOffsetY, int imageOffsetZ,
				uint32_t imageWidth, uint32_t imageHeight, uint32_t imageDepth, ImageAspectBits aspectBits) override;

			void SetRenderPass(SPtr<BaseRenderRenderPass>& renderPass, Rect2D renderArea) override;

			void PipelineBufferMemoryBarrier(PipelineStageBits srcStage, PipelineStageBits dstStage,
				AccessBits srcAccess, AccessBits dstAccess, SPtr<BaseRenderBuffer>& buffer, uint64_t offset, uint64_t size) override;
			void PipelineImageMemoryBarrier(PipelineStageBits srcStage, PipelineStageBits dstStage,
				AccessBits srcAccess, AccessBits dstAccess, ImageLayout oldLayout, ImageLayout newLayout,
				SPtr<BaseRenderImage>& image) override;

			void SetViewport(uint32_t firstViewport, uint32_t viewportCount, Viewport* pViewports) override;
			void SetScissor(uint32_t firstScissor, uint32_t scissorCount, Rect2D* pScissors) override;

			void BindGraphicsPipeline(SPtr<BaseRenderGraphicsPipeline>& graphicsPipeline) override;

			void BindDescriptorSets(PipelineType pipelineType, uint32_t firstSet,
				uint32_t descriptorSetNum, SPtr<BaseRenderDescriptorSet>* descriptorSets) override;
			void BindVertexBuffers(uint32_t bufferNum, SPtr<BaseRenderBuffer>* buffers, uint64_t* bufferOffsets) override;
			void BindIndexBuffer(SPtr<BaseRenderBuffer> buffer, uint64_t bufferOffset) override;

			void Draw(uint32_t vertexCount, uint32_t vertexOffset, uint32_t instanceCount, uint32_t firstInstance) override;
			void DrawIndexed(uint32_t indexCount, uint32_t indexOffset, int32_t vertexOffset, uint32_t instanceCount, uint32_t firstInstance) override;

			void End() override;
			void Submit(SPtr<BaseRenderQueue>& queue,
				uint32_t waitSemaphoreNum, std::pair<SPtr<BaseRenderSemaphore>, PipelineStageBits>* waitSemaphores,
				uint32_t signalSemaphoreNum, SPtr<BaseRenderSemaphore>* signalSemaphores, SPtr<BaseRenderFence> waitFence) override;

		private:
			VulkanCommandBuffer(){ }
			VulkanCommandBuffer(SPtr<VulkanDevice>& device, SPtr<VulkanCommandPool>& commandPool, VkCommandBuffer commandBuffer);

			VkPipelineBindPoint GetVkPipelineBindPoint(PipelineType pipelineType);

			VkCommandBuffer mCommandBuffer;

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

			operator VkCommandPool() const
			{
				return mCommandPool;
			}

		private:
			VkCommandPool mCommandPool;

			VulkanQueueFamily mDesignatedQueueFamily;

			SPtr<VulkanDevice> mDevice_ref;
		};
	}
}