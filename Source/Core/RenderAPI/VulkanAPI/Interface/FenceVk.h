#pragma once

#include "../VulkanAPIHeader.h"

#include "RenderAPI/Interface/Fence.h"

#include "../VkObject.h"
#include "../VulkanLogicalDevice.h"

namespace cube
{
	namespace render
	{
		class FenceVk final : public Fence
		{
		public:
			FenceVk(VkFenceWrapper& fence, Uint32 poolIndex, VulkanFencePool& pool, const char* debugName);
			virtual ~FenceVk();

			VkFence GetHandle() const { return mFence.mObject; }

			virtual void Release() override final;

			virtual FenceWaitResult Wait(float timeout) override final;

		private:
			friend class VulkanFencePool;

			VkFenceWrapper& mFence;
			bool mIsReleased;

			Uint32 mFencePoolIndex;
			VulkanFencePool& mPool;
		};
	} // namespace render
} // namespace cube
