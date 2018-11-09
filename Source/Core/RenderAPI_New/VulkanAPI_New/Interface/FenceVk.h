#pragma once

#include "../VulkanAPIHeader.h"

#include "BaseRenderAPI_New/Interface/Fence.h"

namespace cube
{
	namespace render
	{
		class FenceVk final : public Fence
		{
		public:
			FenceVk(DeviceVk& device, VkFence fence, Uint32 poolIndex, VulkanFencePool& pool);
			virtual ~FenceVk();

			VkFence GetHandle() const { return mFence; }

			virtual void Release() override final;

			virtual FenceWaitResult Wait(float timeout) override final;

		private:
			friend class VulkanFencePool;

			DeviceVk& mDevice;
			VkFence mFence;
			Uint32 mFencePoolIndex;
			VulkanFencePool& mPool;
		};
	} // namespace render
} // namespace cube
