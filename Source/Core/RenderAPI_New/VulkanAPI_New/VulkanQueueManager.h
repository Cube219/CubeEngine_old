﻿#pragma once

#include "VulkanAPIHeader.h"

#include "EngineCore/Thread/MutexLock.h"
#include "VulkanSemaphorePool.h"

namespace cube
{
	namespace render
	{
		class VulkanQueueManager
		{
		public:
			VulkanQueueManager(DeviceVk& device, VulkanPhysicalDevice& physicalDevice,
				VulkanFencePool& fencePool, VulkanSemaphorePool& semaphorePool);
			~VulkanQueueManager();

			SPtr<FenceVk> SubmitCommandList(SPtr<CommandListVk>& commandList);

			Uint32 GetGraphicsQueueFamilyIndex() const { return mGraphicsQueueFamilyIndex; }
			Uint32 GetTransferImmediateQueueFamilyIndex() const { return mTransferImmediateQueueFamilyIndex; }
			Uint32 GetTransferDeferredFamilyIndex() const { return mTransferDeferredQueueFamilyIndex; }
			Uint32 GetComputeQueueFamilyIndex() const { return mComputeQueueFamilyIndex; }

		private:
			bool InitGraphicsQueue();
			bool InitTransferImmediateQueues();
			bool InitTransferDeferredQueues();
			bool InitComputeQueues();

			SPtr<FenceVk> SubmitGraphicsQueue(SPtr<CommandListVk>& commandList);
			SPtr<FenceVk> SubmitTransferImmediateQueue(SPtr<CommandListVk>& commandList);
			SPtr<FenceVk> SubmitTransferDeferredQueue(SPtr<CommandListVk>& commandList);
			SPtr<FenceVk> SubmitComputeQueue(SPtr<CommandListVk>& commandList);

			DeviceVk& mDevice;
			VulkanPhysicalDevice& mPhysicalDevice;
			VulkanFencePool& mFencePool;
			VulkanSemaphorePool& mSemaphorePool;

			VkQueue mGraphicsQueue;
			Uint32 mGraphicsQueueFamilyIndex;
			SPtr<FenceVk> mLastGraphicsFence;
			Vector<VulkanSemaphore> mLastGraphicsSemaphores;

			Vector<VkQueue> mTransferImmediateQueues;
			Uint32 mTransferImmediateQueueFamilyIndex;
			core::Mutex mTransferImmediateCurrentIndexMutex;
			Uint64 mTransferImmediateCurrentIndex = 0;
			core::Mutex mImmediateCompleteSemaphoresMutex;
			Vector<VulkanSemaphore> mImmediateCompleteSemaphores;

			Vector<VkQueue> mTransferDeferredQueues;
			core::Mutex mTransferDeferredCurrentIndexMutex;
			Uint32 mTransferDeferredQueueFamilyIndex;
			Uint64 mTransferDeferredCurrentIndex = 0;

			Vector<VkQueue> mComputeQueues;
			Uint32 mComputeQueueFamilyIndex;
			Uint64 mComputeCurrentIndex = 0;
		};
	} // namespace render
} // namespace render