#include "VulkanFencePool.h"

#include "VulkanUtility.h"
#include "VulkanDebug.h"
#include "VulkanLogicalDevice.h"
#include "Interface/FenceVk.h"
#include "EngineCore/Assertion.h"

namespace cube
{
	namespace render
	{
		VulkanFencePool::VulkanFencePool(SPtr<VulkanLogicalDevice>& device) :
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
				mFences[i] = VkFenceWrapper(VK_NULL_HANDLE, device->shared_from_this());
				res = vkCreateFence(device->GetHandle(), &info, nullptr, &mFences[i].mObject);
				CHECK_VK(res, "Failed to create fence.");

				mIdleFenceIndices[i] = i;
			}
		}

		VulkanFencePool::~VulkanFencePool()
		{
			vkDeviceWaitIdle(mDevice->GetHandle());

			mFences.clear();
		}

		SPtr<FenceVk> VulkanFencePool::GetFence(const char* debugName)
		{
			VkResult res;

			Uint32 index;

			{
				Lock lock(mFencesMutex);

				if(mIdleFenceIndices.empty()) {
					VkFenceWrapper fence(VK_NULL_HANDLE, mDevice);

					VkFenceCreateInfo info;
					info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
					info.pNext = nullptr;
					info.flags = 0;

					res = vkCreateFence(mDevice->GetHandle(), &info, nullptr, &fence.mObject);
					CHECK_VK(res, "Failed to create a new fence.");
					index = SCast(Uint32)(mFences.size());

					mFences.push_back(std::move(fence));
				} else {
					index = mIdleFenceIndices.back();

					mIdleFenceIndices.pop_back();
				}
			}

			vkResetFences(mDevice->GetHandle(), 1, &mFences[index].mObject);
			VulkanDebug::SetObjectName(mDevice->GetHandle(), mFences[index].mObject, debugName);

			return std::make_shared<FenceVk>(mFences[index], index, *this, debugName);
		}

		void VulkanFencePool::FreeFence(FenceVk& fence)
		{
			CHECK(vkGetFenceStatus(fence.mFence.GetVkDevice(), fence.mFence.mObject) == VK_SUCCESS,
				"Cannot free a fence that has not been signaled.");

			if(fence.mIsReleased == true)
				return;

			fence.Release();
			{
				Lock lock(mFencesMutex);

				mIdleFenceIndices.push_back(fence.mFencePoolIndex);
			}
		}
	} // namespace render
} // namespace cube
