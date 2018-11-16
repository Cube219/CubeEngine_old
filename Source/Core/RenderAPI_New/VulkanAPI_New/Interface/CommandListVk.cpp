#include "CommandListVk.h"

#include "../VulkanUtility.h"
#include "../VulkanCommandListPool.h"
#include "BufferVk.h"

namespace cube
{
	namespace render
	{
		CommandListVk::CommandListVk(SPtr<VulkanCommandListPool> pool, VkCommandBuffer commandBuffer,
			CommandListUsage usage, Uint32 submitQueueFamilyIndex) :
			CommandList(usage),
			mPool(pool), mCommandBuffer(commandBuffer), mSubmitQueueFamilyIndex(submitQueueFamilyIndex)
		{
		}

		CommandListVk::~CommandListVk()
		{
			mPool->Free(*this);
		}

		void CommandListVk::Begin()
		{
			VkResult res;

			VkCommandBufferBeginInfo beginInfo;
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			beginInfo.pNext = nullptr;
			beginInfo.flags = 0;
			beginInfo.pInheritanceInfo = nullptr;

			res = vkBeginCommandBuffer(mCommandBuffer, &beginInfo);
			CHECK_VK(res, "Failed to begin the command buffer.");
		}

		void CommandListVk::End()
		{
			VkResult res;

			res = vkEndCommandBuffer(mCommandBuffer);
			CHECK_VK(res, "Failed to end the command buffer.");
		}

		void CommandListVk::CopyBuffer(const Buffer& src, Buffer& dst, Uint64 srcOffset, Uint64 dstOffset, Uint64 size)
		{
			VkBufferCopy copy;
			copy.srcOffset = srcOffset;
			copy.dstOffset = dstOffset;
			copy.size = size;

			vkCmdCopyBuffer(mCommandBuffer, DCast(const BufferVk&)(src).GetHandle(), DCast(BufferVk&)(dst).GetHandle(), 1, &copy);
		}
	} // namespace render
} // namespace cube
