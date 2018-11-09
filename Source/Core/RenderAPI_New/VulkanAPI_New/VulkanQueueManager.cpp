#include "VulkanQueueManager.h"

#include "VulkanUtility.h"
#include "Interface/DeviceVk.h"
#include "Interface/CommandListVk.h"
#include "Interface/FenceVk.h"
#include "VulkanPhysicalDevice.h"
#include "VulkanFencePool.h"
#include "EngineCore/Assertion.h"

namespace cube
{
	namespace render
	{
		VulkanQueueManager::VulkanQueueManager(DeviceVk& device, VulkanPhysicalDevice& physicalDevice,
			VulkanFencePool& fencePool, VulkanSemaphorePool& semaphorePool) :
			mDevice(device), mPhysicalDevice(physicalDevice), mFencePool(fencePool), mSemaphorePool(semaphorePool),
			mLastGraphicsFence(nullptr)
		{
			bool res;
			res = InitGraphicsQueue();
			CHECK(res, "Failed to init a graphics queue.");

			res = InitTransferDeferredQueues();
			CHECK(res, "Failed to init transfer deferred queues.");

			res = InitTransferImmediateQueues();
			CHECK(res, "Failed to init transfer immediate queues.");

			res = InitComputeQueues();
			CHECK(res, "Failed to init compute queues.");
		}

		VulkanQueueManager::~VulkanQueueManager()
		{
		}

		SPtr<FenceVk> VulkanQueueManager::SubmitCommandList(SPtr<CommandListVk>& commandList)
		{
			CommandListUsage usage = commandList->GetUsage();
			switch(usage) {
				case CommandListUsage::Graphics:
					return SubmitGraphicsQueue(commandList);

				case CommandListUsage::TransferImmediate:
					return SubmitTransferImmediateQueue(commandList);

				case CommandListUsage::TransferDeferred:
					return SubmitTransferDeferredQueue(commandList);

				case CommandListUsage::Compute:
					return SubmitComputeQueue(commandList);

				default:
					return nullptr;
			}
		}

		bool VulkanQueueManager::InitGraphicsQueue()
		{
			mGraphicsQueueFamilyIndex = mPhysicalDevice.FindQueueFamilyIndex(VK_QUEUE_GRAPHICS_BIT, 0);
			if(mGraphicsQueueFamilyIndex == UInt32InvalidValue) {
				return false;
			}

			// Only use first queue in queue family
			vkGetDeviceQueue(mDevice.GetHandle(), mGraphicsQueueFamilyIndex, 0, &mGraphicsQueue);
			return true;
		}

		bool VulkanQueueManager::InitTransferImmediateQueues()
		{
			auto& props = mPhysicalDevice.GetQueueFamilyProperties();

			mTransferImmediateQueueFamilyIndex = mPhysicalDevice.FindQueueFamilyIndex(VK_QUEUE_TRANSFER_BIT, 0);
			if(mTransferImmediateQueueFamilyIndex == UInt32InvalidValue) {
				return false;
			}

			Uint32 queueCount = props[mTransferImmediateQueueFamilyIndex].queueCount;
			mTransferImmediateQueues.resize(queueCount);

			for(Uint32 i = 0; i < queueCount; i++) {
				if(mGraphicsQueueFamilyIndex == mTransferImmediateQueueFamilyIndex) {
					// If the queue family is graphics queue family, skip the first queue
					// because it is used by graphics only
					mTransferImmediateQueues.resize(queueCount - 1);
					continue;
				}
				vkGetDeviceQueue(mDevice.GetHandle(), mTransferImmediateQueueFamilyIndex, i, &mTransferImmediateQueues[i]);
			}

			if(mTransferImmediateQueues.size() == 0) {
				// If the queue family is graphics queue family, but it has only one queue,
				// just add its queue
				mTransferImmediateQueues.resize(1);
				vkGetDeviceQueue(mDevice.GetHandle(), mTransferImmediateQueueFamilyIndex, 0, &mTransferImmediateQueues[0]);
			}

			return true;
		}

		bool VulkanQueueManager::InitTransferDeferredQueues()
		{
			auto& props = mPhysicalDevice.GetQueueFamilyProperties();

			// Find queue family that only have TRANSFER bits
			mTransferDeferredQueueFamilyIndex =
				mPhysicalDevice.FindQueueFamilyIndex(VK_QUEUE_TRANSFER_BIT, VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT);
			if(mTransferDeferredQueueFamilyIndex != UInt32InvalidValue) {
				Uint32 queueCount = props[mTransferDeferredQueueFamilyIndex].queueCount;
				mTransferDeferredQueues.resize(queueCount);

				for(Uint32 i = 0; i < queueCount; i++) {
					vkGetDeviceQueue(mDevice.GetHandle(), mTransferDeferredQueueFamilyIndex, i, &mTransferDeferredQueues[i]);
				}
				return true;
			}

			// If not found, find with TRANSFER bits
			mTransferDeferredQueueFamilyIndex = mPhysicalDevice.FindQueueFamilyIndex(VK_QUEUE_TRANSFER_BIT, 0);

			Uint32 queueCount = props[mTransferDeferredQueueFamilyIndex].queueCount;
			mTransferDeferredQueues.resize(queueCount);

			for(Uint32 i = 0; i < queueCount; i++) {
				if(mGraphicsQueueFamilyIndex == mTransferDeferredQueueFamilyIndex) {
					// If the queue family is graphics queue family, skip the first queue
					// because it is used by graphics only
					mTransferDeferredQueues.resize(queueCount - 1);
					continue;
				}
				vkGetDeviceQueue(mDevice.GetHandle(), mTransferDeferredQueueFamilyIndex, i, &mTransferDeferredQueues[i]);
			}

			if(mTransferDeferredQueues.size() == 0) {
				// If the queue family is graphics queue family, but it has only one queue,
				// just add its queue
				mTransferDeferredQueues.resize(1);
				vkGetDeviceQueue(mDevice.GetHandle(), mTransferDeferredQueueFamilyIndex, 0, &mTransferDeferredQueues[0]);
			}

			return true;
		}

		bool VulkanQueueManager::InitComputeQueues()
		{
			auto& props = mPhysicalDevice.GetQueueFamilyProperties();

			// Find queue family that only have COMPUTE bits
			// TODO: Translate (TRANSFER는 포함시키는데, 이는 AMD그래픽카드에서 COMPUTE|TRANSFER 인 QueueFamily를 쓰기 위해서)
			mComputeQueueFamilyIndex =
				mPhysicalDevice.FindQueueFamilyIndex(VK_QUEUE_COMPUTE_BIT, VK_QUEUE_GRAPHICS_BIT);
			if(mComputeQueueFamilyIndex != UInt32InvalidValue) {
				Uint32 queueCount = props[mComputeQueueFamilyIndex].queueCount;
				mComputeQueues.resize(queueCount);

				for(Uint32 i = 0; i < queueCount; i++) {
					vkGetDeviceQueue(mDevice.GetHandle(), mComputeQueueFamilyIndex, i, &mComputeQueues[i]);
				}
				return true;
			}

			// If not found, find with COMPUTE bits
			mComputeQueueFamilyIndex = mPhysicalDevice.FindQueueFamilyIndex(VK_QUEUE_TRANSFER_BIT, 0);

			Uint32 queueCount = props[mComputeQueueFamilyIndex].queueCount;
			mComputeQueues.resize(queueCount);

			for(Uint32 i = 0; i < queueCount; i++) {
				if(mGraphicsQueueFamilyIndex == mComputeQueueFamilyIndex) {
					// If the queue family is graphics queue family, skip the first queue
					// because it is used by graphics only
					mComputeQueues.resize(queueCount - 1);
					continue;
				}
				vkGetDeviceQueue(mDevice.GetHandle(), mComputeQueueFamilyIndex, i, &mComputeQueues[i]);
			}

			if(mComputeQueues.size() == 0) {
				// If the queue family is graphics queue family, but it has only one queue,
				// just add its queue
				mComputeQueues.resize(1);
				vkGetDeviceQueue(mDevice.GetHandle(), mComputeQueueFamilyIndex, 0, &mComputeQueues[0]);
			}

			return true;
		}

		SPtr<FenceVk> VulkanQueueManager::SubmitGraphicsQueue(SPtr<CommandListVk>& commandList)
		{
			CHECK(commandList->GetSubmitQueueFamilyIndex() == mGraphicsQueueFamilyIndex,
				"Submit queue family index in commandList doesn't match graphics queue family index.");

			VkResult res;

			Vector<VulkanSemaphore> waitSemaphores;
			{
				core::Lock lock(mImmediateCompleteSemaphoresMutex);
				
				waitSemaphores = mImmediateCompleteSemaphores;
				mImmediateCompleteSemaphores.clear();
			}

			// Wait for last submit for freeing semaphores used last submit
			FenceWaitResult fenceRes = mLastGraphicsFence->Wait(100000.0f);
			if(fenceRes == FenceWaitResult::Timeout) {
				ASSERTION_FAILED("Failed to submit graphcis queue. Last submit isn't completed too long(over 100000s).");
			}
			if(fenceRes == FenceWaitResult::Error) {
				ASSERTION_FAILED("Failed to submit graphcis queue. Last submit has some error.");
			}
			
			// Free semaphores used last submit
			for(auto& s : mLastGraphicsSemaphores) {
				mSemaphorePool.FreeSemaphore(s);
			}
			mLastGraphicsSemaphores.clear();

			VkCommandBuffer cmdBufToSubmit = commandList->GetHandle();

			// TODO: 나중에 waitStage들을 transfer immediate를 submit할 때 지정할 수 있게 하기
			Vector<VkPipelineStageFlags> waitStages(waitSemaphores.size(), VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT);

			Vector<VkSemaphore> rawSemaphores(waitSemaphores.size());
			for(Uint32 i = 0; i < (Uint32)waitSemaphores.size(); i++) {
				rawSemaphores[i] = waitSemaphores[i].handle;
			}
			VkSubmitInfo submitInfo = {};
			submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			submitInfo.pNext = nullptr;
			submitInfo.waitSemaphoreCount = SCast(Uint32)(rawSemaphores.size());
			submitInfo.pWaitSemaphores = rawSemaphores.data();
			submitInfo.pWaitDstStageMask = waitStages.data();
			submitInfo.commandBufferCount = 1;
			submitInfo.pCommandBuffers = &cmdBufToSubmit;
			submitInfo.signalSemaphoreCount = 0;
			submitInfo.pSignalSemaphores = nullptr;

			SPtr<FenceVk> fence = mFencePool.GetFence();

			res = vkQueueSubmit(mGraphicsQueue, 1, &submitInfo, fence->GetHandle());
			CheckVkResult("Failed to submit command list to the graphics queue.", res);
			
			// Update last fence / semaphores
			mLastGraphicsFence = fence;
			mLastGraphicsSemaphores = waitSemaphores;

			return fence;
		}

		SPtr<FenceVk> VulkanQueueManager::SubmitTransferImmediateQueue(SPtr<CommandListVk>& commandList)
		{
			CHECK(commandList->GetSubmitQueueFamilyIndex() == mTransferImmediateQueueFamilyIndex,
				"Submit queue family index in commandList doesn't match transfer immediate queue family index.");

			VkResult res;

			VkCommandBuffer cmdBufToSubmit = commandList->GetHandle();

			VulkanSemaphore completeSemaphore = mSemaphorePool.GetSemaphore();

			VkSubmitInfo submitInfo = {};
			submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			submitInfo.pNext = nullptr;
			submitInfo.waitSemaphoreCount = 0;
			submitInfo.pWaitSemaphores = nullptr;
			submitInfo.pWaitDstStageMask = nullptr;
			submitInfo.commandBufferCount = 1;
			submitInfo.pCommandBuffers = &cmdBufToSubmit;
			submitInfo.signalSemaphoreCount = 1;
			submitInfo.pSignalSemaphores = &completeSemaphore.handle;

			VkQueue queueToSubmit;
			{
				core::Lock lock(mTransferImmediateCurrentIndexMutex);

				queueToSubmit = mTransferImmediateQueues[mTransferImmediateCurrentIndex++];
				mTransferImmediateCurrentIndex %= mTransferImmediateQueues.size();
			}
			{
				core::Lock lock(mImmediateCompleteSemaphoresMutex);

				mImmediateCompleteSemaphores.push_back(completeSemaphore);
			}

			SPtr<FenceVk> fence = mFencePool.GetFence();

			res = vkQueueSubmit(queueToSubmit, 1, &submitInfo, fence->GetHandle());
			CheckVkResult("Failed to submit command list to the transfer immediate queue.", res);

			return fence;
		}

		SPtr<FenceVk> VulkanQueueManager::SubmitTransferDeferredQueue(SPtr<CommandListVk>& commandList)
		{
			CHECK(commandList->GetSubmitQueueFamilyIndex() == mTransferDeferredQueueFamilyIndex,
				"Submit queue family index in commandList doesn't match transfer deferred queue family index.");

			VkResult res;

			VkCommandBuffer cmdBufToSubmit = commandList->GetHandle();

			VkSubmitInfo submitInfo = {};
			submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			submitInfo.pNext = nullptr;
			submitInfo.waitSemaphoreCount = 0;
			submitInfo.pWaitSemaphores = nullptr;
			submitInfo.pWaitDstStageMask = nullptr;
			submitInfo.commandBufferCount = 1;
			submitInfo.pCommandBuffers = &cmdBufToSubmit;
			submitInfo.signalSemaphoreCount = 0;
			submitInfo.pSignalSemaphores = nullptr;

			VkQueue queueToSubmit;
			{
				core::Lock lock(mTransferDeferredCurrentIndexMutex);
				
				queueToSubmit = mTransferDeferredQueues[mTransferDeferredCurrentIndex++];
				mTransferDeferredCurrentIndex %= mTransferDeferredQueues.size();
			}

			SPtr<FenceVk> fence = mFencePool.GetFence();

			res = vkQueueSubmit(queueToSubmit, 1, &submitInfo, fence->GetHandle());
			CheckVkResult("Failed to submit command list to the transfer deferred queue.", res);
			
			return fence;
		}

		SPtr<FenceVk> VulkanQueueManager::SubmitComputeQueue(SPtr<CommandListVk>& commandList)
		{
			CHECK(commandList->GetSubmitQueueFamilyIndex() == mComputeQueueFamilyIndex,
				"Submit queue family index in commandList doesn't match compute queue family index.");

			ASSERTION_FAILED("Submit compute queue isn't implemented yet.");

			return nullptr;
		}
	} // namespace render
} // namespace render
