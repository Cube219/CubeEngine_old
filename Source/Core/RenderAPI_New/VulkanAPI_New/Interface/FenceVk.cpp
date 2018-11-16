#include "FenceVk.h"

#include "EngineCore/Assertion.h"
#include "../VulkanFencePool.h"

namespace cube
{
	namespace render
	{
		FenceVk::FenceVk(VkFenceWrapper& fence, Uint32 poolIndex, VulkanFencePool& pool) :
			mFence(fence), mFencePoolIndex(poolIndex), mPool(pool)
		{
		}

		FenceVk::~FenceVk()
		{
			mPool.FreeFence(*this);
		}

		void FenceVk::Release()
		{
			mPool.FreeFence(*this);
		}

		FenceWaitResult FenceVk::Wait(float timeout)
		{
			if(mFence.IsEmpty()) {
				ASSERTION_FAILED("Failed to wait the fence. The fence already released.");
				return FenceWaitResult::Error;
			}

			Uint64 nanoSecond = (Uint64)((double)timeout * 1000.0 * 1000.0 * 1000.0);
			VkResult res = vkWaitForFences(mFence.GetVkDevice(), 1, &mFence.mObject, VK_TRUE, nanoSecond);

			switch(res) {
				case VK_SUCCESS:
					return FenceWaitResult::Success;
				case VK_TIMEOUT:
					return FenceWaitResult::Timeout;
				default:
					return FenceWaitResult::Error;
			}
		}
	} // namespace render
} // namespace cube
