#pragma once

#include "VulkanAPIHeader.h"

#include "EngineCore/Thread/MutexLock.h"

namespace cube
{
	namespace render
	{
		class VulkanFencePool
		{
		public:
			VulkanFencePool(DeviceVk& device);
			~VulkanFencePool();

			SPtr<FenceVk> GetFence();
			void FreeFence(FenceVk& fence);

		private:
			DeviceVk& mDevice;

			core::Mutex mFencesMutex;
			Vector<VkFence> mFences;
			Vector<Uint32> mIdleFenceIndices;
		};
	} // namespace render
} // namespace cube
