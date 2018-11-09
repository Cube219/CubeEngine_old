#include "VulkanCommandListPool.h"

#include "VulkanUtility.h"
#include "Interface/DeviceVk.h"
#include "VulkanQueueManager.h"

namespace cube
{
	namespace render
	{
		VulkanCommandListPool::VulkanCommandListPool(SPtr<DeviceVk>& device, UPtr<VulkanQueueManager>& queueManager) :
			mDevice(device)
		{
			VkResult res;

			VkCommandPoolCreateInfo info;
			info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
			info.pNext = nullptr;

			// Graphics command pool
			info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
			mGraphicsQueueFamilyIndex = queueManager->GetGraphicsQueueFamilyIndex();
			info.queueFamilyIndex = mGraphicsQueueFamilyIndex;

			res = vkCreateCommandPool(mDevice->GetHandle(), &info, nullptr, &mGraphicsCommandPool);
			CheckVkResult("Failed to create graphics command pool.", res);

			// Transfer command pool
			info.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
			mTransferImmediateQueueFamilyIndex = queueManager->GetTransferImmediateQueueFamilyIndex();
			info.queueFamilyIndex = mTransferImmediateQueueFamilyIndex;
			
			res = vkCreateCommandPool(mDevice->GetHandle(), &info, nullptr, &mTransferImmediateCommandPool);
			CheckVkResult("Failed to create transfer immediate command pool.", res);

			info.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
			mTransferDeferredQueueFamilyIndex = queueManager->GetTransferDeferredFamilyIndex();
			info.queueFamilyIndex = mTransferDeferredQueueFamilyIndex;

			res = vkCreateCommandPool(mDevice->GetHandle(), &info, nullptr, &mTransferDeferredCommandPool);
			CheckVkResult("Failed to create transfer deferred command pool.", res);

			// Compute command pool
			info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
			mComputeQueueFamilyIndex = queueManager->GetComputeQueueFamilyIndex();
			info.queueFamilyIndex = mComputeQueueFamilyIndex;

			res = vkCreateCommandPool(mDevice->GetHandle(), &info, nullptr, &mComputeCommandPool);
			CheckVkResult("Failed to create compute command pool.", res);
		}

		VulkanCommandListPool::~VulkanCommandListPool()
		{
			vkDestroyCommandPool(mDevice->GetHandle(), mGraphicsCommandPool, nullptr);
			vkDestroyCommandPool(mDevice->GetHandle(), mTransferImmediateCommandPool, nullptr);
			vkDestroyCommandPool(mDevice->GetHandle(), mTransferDeferredCommandPool, nullptr);
			vkDestroyCommandPool(mDevice->GetHandle(), mComputeCommandPool, nullptr);
		}

		SPtr<CommandListVk> VulkanCommandListPool::Allocate(CommandListUsage usage)
		{
			VkCommandBufferAllocateInfo info;
			info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			info.pNext = nullptr;
			info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
			info.commandBufferCount = 1;

			VkCommandBuffer cmdBuf;

			switch(usage) {
				case CommandListUsage::Graphics:
				{
					info.commandPool = mGraphicsCommandPool;

					core::Lock lock(mGraphicsCommandPoolMutex);
					vkAllocateCommandBuffers(mDevice->GetHandle(), nullptr, &cmdBuf);
					lock.release();

					return std::make_shared<CommandListVk>(shared_from_this(), cmdBuf, usage, mGraphicsQueueFamilyIndex);
				}

				case CommandListUsage::TransferImmediate:
				{
					info.commandPool = mTransferImmediateCommandPool;

					core::Lock lock(mTransferImmediateCommandPoolMutex);
					vkAllocateCommandBuffers(mDevice->GetHandle(), nullptr, &cmdBuf);
					lock.release();

					return std::make_shared<CommandListVk>(shared_from_this(), cmdBuf, usage, mTransferImmediateQueueFamilyIndex);
				}

				case CommandListUsage::TransferDeferred:
				{
					info.commandPool = mTransferDeferredCommandPool;

					core::Lock lock(mTransferDeferredCommandPoolMutex);
					vkAllocateCommandBuffers(mDevice->GetHandle(), nullptr, &cmdBuf);
					lock.release();

					return std::make_shared<CommandListVk>(shared_from_this(), cmdBuf, usage, mTransferDeferredQueueFamilyIndex);
				}

				case CommandListUsage::Compute:
				{
					info.commandPool = mComputeCommandPool;

					core::Lock lock(mComputeCommandPoolMutex);
					vkAllocateCommandBuffers(mDevice->GetHandle(), nullptr, &cmdBuf);
					lock.release();

					return std::make_shared<CommandListVk>(shared_from_this(), cmdBuf, usage, mComputeQueueFamilyIndex);
				}
			}

			return nullptr;
		}

		void VulkanCommandListPool::Free(CommandListVk& commandList)
		{
			VkCommandBuffer cmdBuf = commandList.GetHandle();

			switch(commandList.GetUsage()) {
				case CommandListUsage::Graphics:
				{
					core::Lock lock(mGraphicsCommandPoolMutex);
					vkFreeCommandBuffers(mDevice->GetHandle(), mGraphicsCommandPool, 1, &cmdBuf);
					break;
				}

				case CommandListUsage::TransferImmediate:
				{
					core::Lock lock(mTransferImmediateCommandPoolMutex);
					vkFreeCommandBuffers(mDevice->GetHandle(), mTransferImmediateCommandPool, 1, &cmdBuf);
					break;
				}

				case CommandListUsage::TransferDeferred:
				{
					core::Lock lock(mTransferDeferredCommandPoolMutex);
					vkFreeCommandBuffers(mDevice->GetHandle(), mTransferDeferredCommandPool, 1, &cmdBuf);
					break;
				}

				case CommandListUsage::Compute:
				{
					core::Lock lock(mComputeCommandPoolMutex);
					vkFreeCommandBuffers(mDevice->GetHandle(), mComputeCommandPool, 1, &cmdBuf);
					break;
				}
			}
		}
	} // namespace render
} // namespace cube
