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
			VulkanSemaphorePool(SPtr<VulkanLogicalDevice>& device);
			~VulkanSemaphorePool();

			VulkanSemaphore GetSemaphore(const char* debugName = "");
			void FreeSemaphore(VulkanSemaphore& semaphore);

		private:
			SPtr<VulkanLogicalDevice> mDevice;

			Mutex mSemaphoresMutex;
			Vector<VkSemaphore> mSemaphores;
			Vector<Uint32> mIdleSemaphoreIndices;
		};
	} // namespace render
} // namespace cube
