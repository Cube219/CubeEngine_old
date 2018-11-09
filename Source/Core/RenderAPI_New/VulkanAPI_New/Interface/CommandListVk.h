#pragma once

#include "../VulkanAPIHeader.h"

#include "BaseRenderAPI_New/Interface/CommandList.h"

namespace cube
{
	namespace render
	{
		class CommandListVk final : public CommandList
		{
		public:
			CommandListVk(SPtr<VulkanCommandListPool> pool, VkCommandBuffer commandBuffer,
				CommandListUsage usage, Uint32 submitQueueFamilyIndex);
			virtual ~CommandListVk();

			VkCommandBuffer GetHandle() const { return mCommandBuffer; }

			Uint32 GetSubmitQueueFamilyIndex() const { return mSubmitQueueFamilyIndex; }

			virtual void Begin() override final;
			virtual void End() override final;

			virtual void CopyBuffer(SPtr<BufferVk>& src, SPtr<BufferVk>& dst, Uint64 srcOffset, Uint64 dstOffset, Uint64 size) override final;

		private:
			SPtr<VulkanCommandListPool> mPool;
			Uint32 mSubmitQueueFamilyIndex;

			VkCommandBuffer mCommandBuffer;
		};
	} // namespace render
} // namespace cube
