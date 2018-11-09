#pragma once

#include "VulkanAPIHeader.h"

#include "EngineCore/Thread/MutexLock.h"

namespace cube
{
	namespace render
	{
		struct VulkanSemaphore
		{
			VkSemaphore handle;
			Uint32 poolIndex;
		};

		class VulkanSemaphorePool
		{
		public:
			VulkanSemaphorePool(DeviceVk& device);
			~VulkanSemaphorePool();

			VulkanSemaphore GetSemaphore();
			void FreeSemaphore(VulkanSemaphore& semaphore);

		private:
			DeviceVk& mDevice;

			core::Mutex mSemaphoresMutex;
			Vector<VkSemaphore> mSemaphores;
			Vector<Uint32> mIdleSemaphoreIndices;
		};
	} // namespace render
} // namespace cube
