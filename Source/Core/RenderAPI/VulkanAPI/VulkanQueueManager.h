#pragma once

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
			VulkanQueueManager(SPtr<VulkanLogicalDevice>& device, VulkanPhysicalDevice& physicalDevice,
				VulkanFencePool& fencePool, VulkanSemaphorePool& semaphorePool);
			~VulkanQueueManager();

			void AddWaitSemaphoreForGraphics(const VulkanSemaphore& semaphore, VkPipelineStageFlags stageFlags);

			void SubmitCommandList(CommandListVk& commandList);
			SPtr<FenceVk> SubmitCommandListWithFence(CommandListVk& commandList);

			Uint32 GetGraphicsQueueFamilyIndex() const { return mGraphicsQueueFamilyIndex; }
			Uint32 GetTransferImmediateQueueFamilyIndex() const { return mTransferImmediateQueueFamilyIndex; }
			Uint32 GetTransferDeferredFamilyIndex() const { return mTransferDeferredQueueFamilyIndex; }
			Uint32 GetComputeQueueFamilyIndex() const { return mComputeQueueFamilyIndex; }
			VkQueue GetPresentQueue(VkSurfaceKHR surface);

		private:
			bool InitGraphicsQueue();
			bool InitTransferImmediateQueues();
			bool InitTransferDeferredQueues();
			bool InitComputeQueues();

			SPtr<FenceVk> SubmitGraphicsQueue(CommandListVk& commandList, bool getFence);
			SPtr<FenceVk> SubmitTransferImmediateQueue(CommandListVk& commandList, bool getFence);
			SPtr<FenceVk> SubmitTransferDeferredQueue(CommandListVk& commandList, bool getFence);
			SPtr<FenceVk> SubmitComputeQueue(CommandListVk& commandList, bool getFence);

			SPtr<VulkanLogicalDevice>& mDevice;
			VulkanPhysicalDevice& mPhysicalDevice;
			VulkanFencePool& mFencePool;
			VulkanSemaphorePool& mSemaphorePool;

			VkQueue mGraphicsQueue;
			Uint32 mGraphicsQueueFamilyIndex;
			SPtr<FenceVk> mLastGraphicsFence = nullptr;
			Vector<VulkanSemaphore> mLastGraphicsSemaphores;
			Vector<VulkanSemaphore> mWaitSemaphores; // For graphics
			Vector<VkPipelineStageFlags> mWaitSemaphoreStages;

			Vector<VkQueue> mTransferImmediateQueues;
			Uint32 mTransferImmediateQueueFamilyIndex;
			Mutex mTransferImmediateCurrentIndexMutex;
			Uint64 mTransferImmediateCurrentIndex = 0;
			Mutex mImmediateCompleteSemaphoresMutex;
			Vector<VulkanSemaphore> mImmediateCompleteSemaphores;

			Vector<VkQueue> mTransferDeferredQueues;
			Mutex mTransferDeferredCurrentIndexMutex;
			Uint32 mTransferDeferredQueueFamilyIndex;
			Uint64 mTransferDeferredCurrentIndex = 0;

			Vector<VkQueue> mComputeQueues;
			Uint32 mComputeQueueFamilyIndex;
			Uint64 mComputeCurrentIndex = 0;
		};
	} // namespace render
} // namespace render
