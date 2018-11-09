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
			VulkanCommandListPool(SPtr<DeviceVk>& device, UPtr<VulkanQueueManager>& queueManager);
			~VulkanCommandListPool();

			SPtr<CommandListVk> Allocate(CommandListUsage usage);
			void Free(CommandListVk& commandList);

		private:
			SPtr<DeviceVk> mDevice;

			core::Mutex mGraphicsCommandPoolMutex;
			VkCommandPool mGraphicsCommandPool;
			Uint32 mGraphicsQueueFamilyIndex;

			core::Mutex mTransferImmediateCommandPoolMutex;
			VkCommandPool mTransferImmediateCommandPool;
			Uint32 mTransferImmediateQueueFamilyIndex;

			core::Mutex mTransferDeferredCommandPoolMutex;
			VkCommandPool mTransferDeferredCommandPool;
			Uint32 mTransferDeferredQueueFamilyIndex;

			core::Mutex mComputeCommandPoolMutex;
			VkCommandPool mComputeCommandPool;
			Uint32 mComputeQueueFamilyIndex;
		};
	} // namespace render
} // namespace cube
