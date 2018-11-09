#include "VulkanSemaphorePool.h"

#include "VulkanUtility.h"
#include "Interface/DeviceVk.h"

namespace cube
{
	namespace render
	{
		VulkanSemaphorePool::VulkanSemaphorePool(DeviceVk& device) : 
			mDevice(device)
		{
			VkResult res;

			// Create 20 Semaphores
			constexpr Uint32 initSize = 20;
			mSemaphores.resize(initSize);
			mIdleSemaphoreIndices.resize(initSize);

			VkSemaphoreCreateInfo info;
			info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
			info.pNext = nullptr;
			info.flags = 0;

			for(Uint32 i = 0; i < initSize; i++) {
				res = vkCreateSemaphore(device.GetHandle(), &info, nullptr, &mSemaphores[i]);
				CheckVkResult("Failed to create semaphore.", res);
				mIdleSemaphoreIndices[i] = i;
			}
		}

		VulkanSemaphorePool::~VulkanSemaphorePool()
		{
			vkDeviceWaitIdle(mDevice.GetHandle());

			for(auto s : mSemaphores) {
				vkDestroySemaphore(mDevice.GetHandle(), s, nullptr);
			}
		}

		VulkanSemaphore VulkanSemaphorePool::GetSemaphore()
		{
			VkResult res;

			VulkanSemaphore semaphore;
			{
				core::Lock lock(mSemaphoresMutex);

				if(mIdleSemaphoreIndices.empty()) {
					VkSemaphoreCreateInfo info;
					info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
					info.pNext = nullptr;
					info.flags = 0;

					res = vkCreateSemaphore(mDevice.GetHandle(), &info, nullptr, &semaphore.handle);
					CheckVkResult("Failed to create a new semaphore.", res);
					semaphore.poolIndex = mSemaphores.size();

					mSemaphores.push_back(semaphore.handle);

				} else {
					semaphore.poolIndex = mIdleSemaphoreIndices.back();
					semaphore.handle = mSemaphores[semaphore.poolIndex];

					mIdleSemaphoreIndices.pop_back();
				}
			}

			return semaphore;
		}

		void VulkanSemaphorePool::FreeSemaphore(VulkanSemaphore& semaphore)
		{
			if(semaphore.handle == 0 || mSemaphores.empty())
				return;

			semaphore.handle = 0;
			{
				core::Lock lock(mSemaphoresMutex);

				mIdleSemaphoreIndices.push_back(semaphore.poolIndex);
			}
		}
	} // namespace render
} // namespace cube
