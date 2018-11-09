#include "VulkanFencePool.h"

#include "VulkanUtility.h"
#include "Interface/DeviceVk.h"
#include "Interface/FenceVk.h"
#include "EngineCore/Assertion.h"

namespace cube
{
	namespace render
	{
		VulkanFencePool::VulkanFencePool(DeviceVk& device) :
			mDevice(device)
		{
			VkResult res;

			// Create 10 Fences
			constexpr Uint32 initSize = 10;
			mFences.resize(initSize);
			mIdleFenceIndices.resize(initSize);

			VkFenceCreateInfo info;
			info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
			info.pNext = nullptr;
			info.flags = 0;

			for(Uint32 i = 0; i < initSize; i++) {
				res = vkCreateFence(device.GetHandle(), &info, nullptr, &mFences[i]);
				CheckVkResult("Failed to create fence.", res);
				mIdleFenceIndices[i] = i;
			}
		}

		VulkanFencePool::~VulkanFencePool()
		{
			vkDeviceWaitIdle(mDevice.GetHandle());

			for(auto f : mFences) {
				vkDestroyFence(mDevice.GetHandle(), f, nullptr);
			}
		}

		SPtr<FenceVk> VulkanFencePool::GetFence()
		{
			VkResult res;

			VkFence fence = 0;
			Uint32 index;

			{
				core::Lock lock(mFencesMutex);

				if(mIdleFenceIndices.empty()) {
					VkFenceCreateInfo info;
					info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
					info.pNext = nullptr;
					info.flags = 0;

					res = vkCreateFence(mDevice.GetHandle(), &info, nullptr, &fence);
					CheckVkResult("Failed to create a new fence.", res);
					index = SCast(Uint32)(mFences.size());

					mFences.push_back(fence);
				} else {
					index = mIdleFenceIndices.back();
					fence = mFences[index];

					mIdleFenceIndices.pop_back();
				}
			}

			vkResetFences(mDevice.GetHandle(), 1, &fence);
			return std::make_shared<FenceVk>(mDevice, fence, index, *this);
		}

		void VulkanFencePool::FreeFence(FenceVk& fence)
		{
			if(fence.mFence == nullptr || mFences.empty())
				return;

			fence.mFence = nullptr;
			{
				core::Lock lock(mFencesMutex);

				mIdleFenceIndices.push_back(fence.mFencePoolIndex);
			}
		}
	} // namespace render
} // namespace cube
