#pragma once

#include "VulkanAPIHeader.h"

#include "EngineCore/Thread/MutexLock.h"
#include "Interface/CommandListVk.h"

namespace cube
{
	namespace render
	{
		class VulkanCommandListPool : public std::enable_shared_from_this<VulkanCommandListPool>
		{
		public:
			VulkanCommandListPool(SPtr<VulkanLogicalDevice>& device, VulkanQueueManager& queueManager);
			~VulkanCommandListPool();

			SPtr<CommandListVk> Allocate(CommandListUsage usage, Uint32 threadIndex, bool isSub, const char* debugName = "");
			void Free(CommandListVk& commandList);

		private:
			SPtr<VulkanLogicalDevice> mDevice;

			static constexpr Uint32 maxThreadNum = 8;

			Array<Mutex, maxThreadNum> mGraphicsCommandPoolMutexes;
			Array<VkCommandPool, maxThreadNum> mGraphicsCommandPools;
			Uint32 mGraphicsQueueFamilyIndex;

			Array<Mutex, maxThreadNum> mTransferImmediateCommandPoolMutexes;
			Array<VkCommandPool, maxThreadNum> mTransferImmediateCommandPools;
			Uint32 mTransferImmediateQueueFamilyIndex;

			Array<Mutex, maxThreadNum> mTransferDeferredCommandPoolMutexes;
			Array<VkCommandPool, maxThreadNum> mTransferDeferredCommandPools;
			Uint32 mTransferDeferredQueueFamilyIndex;

			Array<Mutex, maxThreadNum> mComputeCommandPoolMutexes;
			Array<VkCommandPool, maxThreadNum> mComputeCommandPools;
			Uint32 mComputeQueueFamilyIndex;
		};
	} // namespace render
} // namespace cube
