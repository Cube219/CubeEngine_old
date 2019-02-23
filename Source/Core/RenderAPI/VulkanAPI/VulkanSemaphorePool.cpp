#include "VulkanSemaphorePool.h"

#include "VulkanUtility.h"
#include "VulkanDebug.h"
#include "VulkanLogicalDevice.h"

namespace cube
{
	namespace render
	{
		VulkanSemaphorePool::VulkanSemaphorePool(SPtr<VulkanLogicalDevice>& device) :
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
				res = vkCreateSemaphore(device->GetHandle(), &info, nullptr, &mSemaphores[i]);
				CHECK_VK(res, "Failed to create semaphore.");
				mIdleSemaphoreIndices[i] = i;
			}
		}

		VulkanSemaphorePool::~VulkanSemaphorePool()
		{
			vkDeviceWaitIdle(mDevice->GetHandle());

			for(auto s : mSemaphores) {
				vkDestroySemaphore(mDevice->GetHandle(), s, nullptr);
			}
		}

		VulkanSemaphore VulkanSemaphorePool::GetSemaphore(const char* debugName)
		{
			VkResult res;

			VulkanSemaphore semaphore;
			{
				Lock lock(mSemaphoresMutex);

				if(mIdleSemaphoreIndices.empty()) {
					VkSemaphoreCreateInfo info;
					info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
					info.pNext = nullptr;
					info.flags = 0;

					res = vkCreateSemaphore(mDevice->GetHandle(), &info, nullptr, &semaphore.handle);
					CHECK_VK(res, "Failed to create a new semaphore.");
					semaphore.poolIndex = SCast(Uint32)(mSemaphores.size());

					mSemaphores.push_back(semaphore.handle);

				} else {
					semaphore.poolIndex = mIdleSemaphoreIndices.back();
					semaphore.handle = mSemaphores[semaphore.poolIndex];

					mIdleSemaphoreIndices.pop_back();
				}
			}

			VulkanDebug::SetObjectName(mDevice->GetHandle(), semaphore.handle, debugName);

			return semaphore;
		}

		void VulkanSemaphorePool::FreeSemaphore(VulkanSemaphore& semaphore)
		{
			if(semaphore.handle == 0 || mSemaphores.empty())
				return;

			semaphore.handle = 0;
			{
				Lock lock(mSemaphoresMutex);

				mIdleSemaphoreIndices.push_back(semaphore.poolIndex);
			}
		}
	} // namespace render
} // namespace cube
