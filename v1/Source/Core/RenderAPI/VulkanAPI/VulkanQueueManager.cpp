#include "VulkanQueueManager.h"

#include "VulkanUtility.h"
#include "Interface/CommandListVk.h"
#include "Interface/FenceVk.h"
#include "VulkanPhysicalDevice.h"
#include "VulkanLogicalDevice.h"
#include "VulkanFencePool.h"
#include "EngineCore/Assertion.h"

namespace cube
{
	namespace render
	{
		VulkanQueueManager::VulkanQueueManager(SPtr<VulkanLogicalDevice>& device, VulkanPhysicalDevice& physicalDevice,
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

		void VulkanQueueManager::AddWaitSemaphoreForGraphics(const VulkanSemaphore& semaphore, VkPipelineStageFlags stageFlags)
		{
			mWaitSemaphores.push_back(semaphore);
			mWaitSemaphoreStages.push_back(stageFlags);
		}

		void VulkanQueueManager::SubmitCommandList(CommandListVk& commandList)
		{
			CommandListUsage usage = commandList.GetUsage();
			switch(usage) {
				case CommandListUsage::Graphics:
					SubmitGraphicsQueue(commandList, false);
					break;

				case CommandListUsage::TransferImmediate:
					SubmitTransferImmediateQueue(commandList, false);
					break;

				case CommandListUsage::TransferDeferred:
					SubmitTransferDeferredQueue(commandList, false);
					break;

				case CommandListUsage::Compute:
					SubmitComputeQueue(commandList, false);
					break;

				default:
					return;
			}
		}

		SPtr<FenceVk> VulkanQueueManager::SubmitCommandListWithFence(CommandListVk& commandList)
		{
			CommandListUsage usage = commandList.GetUsage();
			switch(usage) {
			case CommandListUsage::Graphics:
				return SubmitGraphicsQueue(commandList, true);

			case CommandListUsage::TransferImmediate:
				return SubmitTransferImmediateQueue(commandList, true);

			case CommandListUsage::TransferDeferred:
				return SubmitTransferDeferredQueue(commandList, true);

			case CommandListUsage::Compute:
				return SubmitComputeQueue(commandList, true);

			default:
				return nullptr;
			}
		}

		VkQueue VulkanQueueManager::GetPresentQueue(VkSurfaceKHR surface)
		{
			VkResult res;
			Uint32 presentQueueFamilyIndex = Uint32InvalidValue;

			// Check compute queue family if it is supported to present
			VkBool32 isSupported;
			res = vkGetPhysicalDeviceSurfaceSupportKHR(mPhysicalDevice.GetHandle(), mComputeQueueFamilyIndex, surface, &isSupported);
			CHECK_VK(res, "Failed to get if compute queue family supports present.");
			if(isSupported == VK_TRUE) {
				presentQueueFamilyIndex = SCast(Uint32)(mComputeCurrentIndex);
			} else {
				// If not, check graphics queue family
				res = vkGetPhysicalDeviceSurfaceSupportKHR(mPhysicalDevice.GetHandle(), mGraphicsQueueFamilyIndex, surface, &isSupported);
				CHECK_VK(res, "Failed to get if graphics queue family supports present.");
				if(isSupported == VK_TRUE) {
					presentQueueFamilyIndex = mGraphicsQueueFamilyIndex;
				} else {
					// If not, try to find present queue family
					Uint32 queueFamilySize = SCast(Uint32)(mPhysicalDevice.GetQueueFamilyProperties().size());
					for(Uint32 i = 0; i < queueFamilySize; i++) {
						res = vkGetPhysicalDeviceSurfaceSupportKHR(mPhysicalDevice.GetHandle(), i, surface, &isSupported);
						CHECK_VK(res, "Failed to get the queue family to supports present.");

						if(isSupported == VK_TRUE) {
							presentQueueFamilyIndex = i;
							break;
						}
					}
				}
			}
			CHECK(presentQueueFamilyIndex != Uint32InvalidValue, "There's no queue families that support present.");

			VkQueue queue;
			if(presentQueueFamilyIndex != mGraphicsQueueFamilyIndex) {
				vkGetDeviceQueue(mDevice->GetHandle(), presentQueueFamilyIndex, 0, &queue);
			} else {
				Uint32 queueIndex = 0;
				// If the queue family is graphics queue family and it has two or more queue count,
				// select the second queue because the first queue is used by graphics only
				if(mPhysicalDevice.GetQueueFamilyProperties()[presentQueueFamilyIndex].queueCount > 1)
					queueIndex = 1;

				vkGetDeviceQueue(mDevice->GetHandle(), presentQueueFamilyIndex, queueIndex, &queue);
			}

			return queue;
		}

		bool VulkanQueueManager::InitGraphicsQueue()
		{
			mGraphicsQueueFamilyIndex = mPhysicalDevice.FindQueueFamilyIndex(VK_QUEUE_GRAPHICS_BIT, 0);
			if(mGraphicsQueueFamilyIndex == Uint32InvalidValue) {
				return false;
			}

			// Only use the first queue in queue family
			vkGetDeviceQueue(mDevice->GetHandle(), mGraphicsQueueFamilyIndex, 0, &mGraphicsQueue);

			return true;
		}

		bool VulkanQueueManager::InitTransferImmediateQueues()
		{
			auto& props = mPhysicalDevice.GetQueueFamilyProperties();

			mTransferImmediateQueueFamilyIndex = mPhysicalDevice.FindQueueFamilyIndex(VK_QUEUE_TRANSFER_BIT, 0);
			if(mTransferImmediateQueueFamilyIndex == Uint32InvalidValue) {
				return false;
			}

			Uint32 queueCount = props[mTransferImmediateQueueFamilyIndex].queueCount;
			mTransferImmediateQueues.resize(queueCount);

			Uint32 queueIndex = 0;
			for(Uint32 i = 0; i < queueCount; i++) {
				if(i == 0 && mGraphicsQueueFamilyIndex == mTransferImmediateQueueFamilyIndex) {
					// If the queue family is graphics queue family, skip the first queue
					// because it is used by graphics only
					mTransferImmediateQueues.resize(queueCount - 1);
					continue;
				}
				vkGetDeviceQueue(mDevice->GetHandle(), mTransferImmediateQueueFamilyIndex, i, &mTransferImmediateQueues[queueIndex]);
				queueIndex++;
			}

			if(mTransferImmediateQueues.size() == 0) {
				// If the queue family is graphics queue family, but it has only one queue,
				// just add its queue
				mTransferImmediateQueues.resize(1);
				vkGetDeviceQueue(mDevice->GetHandle(), mTransferImmediateQueueFamilyIndex, 0, &mTransferImmediateQueues[0]);
			}

			return true;
		}

		bool VulkanQueueManager::InitTransferDeferredQueues()
		{
			auto& props = mPhysicalDevice.GetQueueFamilyProperties();

			// Find queue family that only have TRANSFER bits
			mTransferDeferredQueueFamilyIndex =
				mPhysicalDevice.FindQueueFamilyIndex(VK_QUEUE_TRANSFER_BIT, VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT);
			if(mTransferDeferredQueueFamilyIndex != Uint32InvalidValue) {
				Uint32 queueCount = props[mTransferDeferredQueueFamilyIndex].queueCount;
				mTransferDeferredQueues.resize(queueCount);

				for(Uint32 i = 0; i < queueCount; i++) {
					vkGetDeviceQueue(mDevice->GetHandle(), mTransferDeferredQueueFamilyIndex, i, &mTransferDeferredQueues[i]);
				}
				return true;
			}

			// If not found, find with TRANSFER bits
			mTransferDeferredQueueFamilyIndex = mPhysicalDevice.FindQueueFamilyIndex(VK_QUEUE_TRANSFER_BIT, 0);
			if(mTransferDeferredQueueFamilyIndex == Uint32InvalidValue)
				return false;

			Uint32 queueCount = props[mTransferDeferredQueueFamilyIndex].queueCount;
			mTransferDeferredQueues.resize(queueCount);

			Uint32 queueIndex = 0;
			for(Uint32 i = 0; i < queueCount; i++) {
				if(i == 0 && mGraphicsQueueFamilyIndex == mTransferDeferredQueueFamilyIndex) {
					// If the queue family is graphics queue family, skip the first queue
					// because it is used by graphics only
					mTransferDeferredQueues.resize(queueCount - 1);
					continue;
				}
				vkGetDeviceQueue(mDevice->GetHandle(), mTransferDeferredQueueFamilyIndex, i, &mTransferDeferredQueues[queueIndex]);
				queueIndex++;
			}

			if(mTransferDeferredQueues.size() == 0) {
				// If the queue family is graphics queue family, but it has only one queue,
				// just add its queue
				mTransferDeferredQueues.resize(1);
				vkGetDeviceQueue(mDevice->GetHandle(), mTransferDeferredQueueFamilyIndex, 0, &mTransferDeferredQueues[0]);
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
			if(mComputeQueueFamilyIndex != Uint32InvalidValue) {
				Uint32 queueCount = props[mComputeQueueFamilyIndex].queueCount;
				mComputeQueues.resize(queueCount);

				for(Uint32 i = 0; i < queueCount; i++) {
					vkGetDeviceQueue(mDevice->GetHandle(), mComputeQueueFamilyIndex, i, &mComputeQueues[i]);
				}
				return true;
			}

			// If not found, find with COMPUTE bits
			mComputeQueueFamilyIndex = mPhysicalDevice.FindQueueFamilyIndex(VK_QUEUE_COMPUTE_BIT, 0);
			if(mComputeQueueFamilyIndex == Uint32InvalidValue)
				return false;

			Uint32 queueCount = props[mComputeQueueFamilyIndex].queueCount;
			mComputeQueues.resize(queueCount);

			Uint32 queueIndex = 0;
			for(Uint32 i = 0; i < queueCount; i++) {
				if(i == 0 && mGraphicsQueueFamilyIndex == mComputeQueueFamilyIndex) {
					// If the queue family is graphics queue family, skip the first queue
					// because it is used by graphics only
					mComputeQueues.resize(queueCount - 1);
					continue;
				}
				vkGetDeviceQueue(mDevice->GetHandle(), mComputeQueueFamilyIndex, i, &mComputeQueues[queueIndex]);
				queueIndex++;
			}

			if(mComputeQueues.size() == 0) {
				// If the queue family is graphics queue family, but it has only one queue,
				// just add its queue
				mComputeQueues.resize(1);
				vkGetDeviceQueue(mDevice->GetHandle(), mComputeQueueFamilyIndex, 0, &mComputeQueues[0]);
			}

			return true;
		}

		SPtr<FenceVk> VulkanQueueManager::SubmitGraphicsQueue(CommandListVk& commandList, bool getFence)
		{
			CHECK(commandList.GetSubmitQueueFamilyIndex() == mGraphicsQueueFamilyIndex,
				"Submit queue family index in commandList doesn't match graphics queue family index.");

			VkResult res;

			{
				Lock lock(mImmediateCompleteSemaphoresMutex);
				
				// Append
				mWaitSemaphores.insert(mWaitSemaphores.cend(), mImmediateCompleteSemaphores.begin(), mImmediateCompleteSemaphores.end());
				// TODO: 나중에 waitStage들을 transfer immediate를 submit할 때 지정할 수 있게 하기
				mWaitSemaphoreStages.insert(mWaitSemaphoreStages.cend(), mImmediateCompleteSemaphores.size(), VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT);
				mImmediateCompleteSemaphores.clear();
			}

			// Wait for last submit for freeing semaphores used last submit
			if(mLastGraphicsFence != nullptr) {
				FenceWaitResult fenceRes = mLastGraphicsFence->Wait(100000.0f);
				if(fenceRes == FenceWaitResult::Timeout) {
					ASSERTION_FAILED("Failed to submit graphcis queue. Last submit isn't completed too long(over 100000s).");
				}
				if(fenceRes == FenceWaitResult::Error) {
					ASSERTION_FAILED("Failed to submit graphcis queue. Last submit has some error.");
				}
			}
			
			// Free semaphores used last submit
			for(auto& s : mLastGraphicsSemaphores) {
				mSemaphorePool.FreeSemaphore(s);
			}
			mLastGraphicsSemaphores.clear();

			VkCommandBuffer cmdBufToSubmit = commandList.GetHandle();

			Vector<VkSemaphore> waitRawSemaphores(mWaitSemaphores.size());
			for(Uint32 i = 0; i < (Uint32)mWaitSemaphores.size(); i++) {
				waitRawSemaphores[i] = mWaitSemaphores[i].handle;
			}
			VkSubmitInfo submitInfo = {};
			submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			submitInfo.pNext = nullptr;
			submitInfo.waitSemaphoreCount = SCast(Uint32)(waitRawSemaphores.size());
			submitInfo.pWaitSemaphores = waitRawSemaphores.data();
			submitInfo.pWaitDstStageMask = mWaitSemaphoreStages.data();
			submitInfo.commandBufferCount = 1;
			submitInfo.pCommandBuffers = &cmdBufToSubmit;
			submitInfo.signalSemaphoreCount = 0;
			submitInfo.pSignalSemaphores = nullptr;

			SPtr<FenceVk> fence;
			VkFence vkFence;
			if(getFence == true) {
				fence = mFencePool.GetFence("Fence to complete to submit grpahics queue");
				vkFence = fence->GetHandle();
			} else {
				fence = nullptr;
				vkFence = VK_NULL_HANDLE;
			}

			res = vkQueueSubmit(mGraphicsQueue, 1, &submitInfo, vkFence);
			CHECK_VK(res, "Failed to submit command list to the graphics queue.");
			
			// Update last fence / semaphores
			mLastGraphicsFence = fence;
			mLastGraphicsSemaphores = mWaitSemaphores;

			// Clear wait semaphores (But not release semaphore)
			mWaitSemaphores.clear();
			mWaitSemaphoreStages.clear();

			return fence;
		}

		SPtr<FenceVk> VulkanQueueManager::SubmitTransferImmediateQueue(CommandListVk& commandList, bool getFence)
		{
			CHECK(commandList.GetSubmitQueueFamilyIndex() == mTransferImmediateQueueFamilyIndex,
				"Submit queue family index in commandList doesn't match transfer immediate queue family index.");

			VkResult res;

			VkCommandBuffer cmdBufToSubmit = commandList.GetHandle();

			VulkanSemaphore completeSemaphore = mSemaphorePool.GetSemaphore("Semaphore to complete to submit transfer immediate");

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
				Lock lock(mTransferImmediateCurrentIndexMutex);

				queueToSubmit = mTransferImmediateQueues[mTransferImmediateCurrentIndex++];
				mTransferImmediateCurrentIndex %= mTransferImmediateQueues.size();
			}
			{
				Lock lock(mImmediateCompleteSemaphoresMutex);

				mImmediateCompleteSemaphores.push_back(completeSemaphore);
			}

			SPtr<FenceVk> fence;
			VkFence vkFence;
			if(getFence == true) {
				fence = mFencePool.GetFence("Fence to complete to submit transfer immediate");
				vkFence = fence->GetHandle();;
			} else {
				fence = nullptr;
				vkFence = VK_NULL_HANDLE;
			}

			res = vkQueueSubmit(queueToSubmit, 1, &submitInfo, vkFence);
			CHECK_VK(res, "Failed to submit command list to the transfer immediate queue.");

			return fence;
		}

		SPtr<FenceVk> VulkanQueueManager::SubmitTransferDeferredQueue(CommandListVk& commandList, bool getFence)
		{
			CHECK(commandList.GetSubmitQueueFamilyIndex() == mTransferDeferredQueueFamilyIndex,
				"Submit queue family index in commandList doesn't match transfer deferred queue family index.");

			VkResult res;

			VkCommandBuffer cmdBufToSubmit = commandList.GetHandle();

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
				Lock lock(mTransferDeferredCurrentIndexMutex);
				
				queueToSubmit = mTransferDeferredQueues[mTransferDeferredCurrentIndex++];
				mTransferDeferredCurrentIndex %= mTransferDeferredQueues.size();
			}

			SPtr<FenceVk> fence;
			VkFence vkFence;
			if(getFence == true) {
				fence = mFencePool.GetFence("Fence to complete to submit transfer deferred");
				vkFence = fence->GetHandle();
			} else {
				fence = nullptr;
				vkFence = VK_NULL_HANDLE;
			}

			res = vkQueueSubmit(queueToSubmit, 1, &submitInfo, vkFence);
			CHECK_VK(res, "Failed to submit command list to the transfer deferred queue.");
			
			return fence;
		}

		SPtr<FenceVk> VulkanQueueManager::SubmitComputeQueue(CommandListVk& commandList, bool getFence)
		{
			CHECK(commandList.GetSubmitQueueFamilyIndex() == mComputeQueueFamilyIndex,
				"Submit queue family index in commandList doesn't match compute queue family index.");

			ASSERTION_FAILED("Submit compute queue isn't implemented yet.");

			return nullptr;
		}
	} // namespace render
} // namespace render
