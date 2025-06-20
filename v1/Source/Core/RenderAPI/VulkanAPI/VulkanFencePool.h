#pragma once

#include "VulkanAPIHeader.h"

#include "VkObject.h"
#include "EngineCore/Thread/MutexLock.h"

namespace cube
{
	namespace render
	{
		class VulkanFencePool
		{
		public:
			VulkanFencePool(SPtr<VulkanLogicalDevice>& device);
			~VulkanFencePool();

			SPtr<FenceVk> GetFence(const char* debugName = "");
			void FreeFence(FenceVk& fence);

		private:
			SPtr<VulkanLogicalDevice> mDevice;

			Mutex mFencesMutex;
			Vector<VkFenceWrapper> mFences;
			Vector<Uint32> mIdleFenceIndices;
		};
	} // namespace render
} // namespace cube
