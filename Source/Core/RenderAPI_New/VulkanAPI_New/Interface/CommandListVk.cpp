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
			CheckVkResult("Failed to begin the command buffer.", res);
		}

		void CommandListVk::End()
		{
			VkResult res;

			res = vkEndCommandBuffer(mCommandBuffer);
			CheckVkResult("Failed to end the command buffer.", res);
		}

		void CommandListVk::CopyBuffer(SPtr<BufferVk>& src, SPtr<BufferVk>& dst, Uint64 srcOffset, Uint64 dstOffset, Uint64 size)
		{
			VkBufferCopy copy;
			copy.srcOffset = srcOffset;
			copy.dstOffset = dstOffset;
			copy.size = size;

			vkCmdCopyBuffer(mCommandBuffer, src->GetHandle(), dst->GetHandle(), 1, &copy);
		}
	} // namespace render
} // namespace cube
