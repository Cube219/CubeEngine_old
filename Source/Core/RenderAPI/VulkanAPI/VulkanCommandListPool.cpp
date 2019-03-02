#include "VulkanCommandListPool.h"

#include "VulkanDebug.h"
#include "VulkanUtility.h"
#include "VulkanLogicalDevice.h"
#include "VulkanQueueManager.h"
#include "EngineCore/Assertion.h"

namespace cube
{
	namespace render
	{
		VulkanCommandListPool::VulkanCommandListPool(SPtr<VulkanLogicalDevice>& device, VulkanQueueManager& queueManager) :
			mDevice(device)
		{
			VkResult res;

			VkCommandPoolCreateInfo info;
			info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
			info.pNext = nullptr;

			// Graphics command pool
			info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
			mGraphicsQueueFamilyIndex = queueManager.GetGraphicsQueueFamilyIndex();
			info.queueFamilyIndex = mGraphicsQueueFamilyIndex;

			for(Uint32 i = 0; i < maxThreadNum; i++) {
				res = vkCreateCommandPool(mDevice->GetHandle(), &info, nullptr, &mGraphicsCommandPools[i]);
				CHECK_VK(res, "Failed to create graphics command pool.");
				
				VulkanDebug::SetObjectName(mDevice->GetHandle(), mGraphicsCommandPools[i],
					fmt::format("GraphicsCommandPool[{0}]", i).c_str());
			}

			// Transfer command pool
			info.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
			mTransferImmediateQueueFamilyIndex = queueManager.GetTransferImmediateQueueFamilyIndex();
			info.queueFamilyIndex = mTransferImmediateQueueFamilyIndex;
			
			for(Uint32 i = 0; i < maxThreadNum; i++) {
				res = vkCreateCommandPool(mDevice->GetHandle(), &info, nullptr, &mTransferImmediateCommandPools[i]);
				CHECK_VK(res, "Failed to create transfer immediate command pool.");

				VulkanDebug::SetObjectName(mDevice->GetHandle(), mTransferImmediateCommandPools[i],
					fmt::format("TransferImmediateCommandPool[{0}]", i).c_str());
			}

			info.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
			mTransferDeferredQueueFamilyIndex = queueManager.GetTransferDeferredFamilyIndex();
			info.queueFamilyIndex = mTransferDeferredQueueFamilyIndex;

			for(Uint32 i = 0; i < maxThreadNum; i++) {
				res = vkCreateCommandPool(mDevice->GetHandle(), &info, nullptr, &mTransferDeferredCommandPools[i]);
				CHECK_VK(res, "Failed to create transfer deferred command pool.");

				VulkanDebug::SetObjectName(mDevice->GetHandle(), mTransferDeferredCommandPools[i],
					fmt::format("TransferDeferredCommandPool[{0}]", i).c_str());
			}

			// Compute command pool
			info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
			mComputeQueueFamilyIndex = queueManager.GetComputeQueueFamilyIndex();
			info.queueFamilyIndex = mComputeQueueFamilyIndex;

			for(Uint32 i = 0; i < maxThreadNum; i++) {
				res = vkCreateCommandPool(mDevice->GetHandle(), &info, nullptr, &mComputeCommandPools[i]);
				CHECK_VK(res, "Failed to create compute command pool.");

				VulkanDebug::SetObjectName(mDevice->GetHandle(), mComputeCommandPools[i],
					fmt::format("ComputeCommandPool[{0}]", i).c_str());
			}
		}

		VulkanCommandListPool::~VulkanCommandListPool()
		{
			for(Uint32 i = 0; i < maxThreadNum; i++) {
				vkDestroyCommandPool(mDevice->GetHandle(), mGraphicsCommandPools[i], nullptr);
				vkDestroyCommandPool(mDevice->GetHandle(), mTransferImmediateCommandPools[i], nullptr);
				vkDestroyCommandPool(mDevice->GetHandle(), mTransferDeferredCommandPools[i], nullptr);
				vkDestroyCommandPool(mDevice->GetHandle(), mComputeCommandPools[i], nullptr);
			}
		}

		SPtr<CommandListVk> VulkanCommandListPool::Allocate(CommandListUsage usage, Uint32 threadIndex, bool isSub, const char* debugName)
		{
			VkCommandBufferAllocateInfo info;
			info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			info.pNext = nullptr;
			if(isSub == false)
				info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
			else
				info.level = VK_COMMAND_BUFFER_LEVEL_SECONDARY;
			info.commandBufferCount = 1;

			SPtr<CommandListVk> cmd;
			VkCommandBuffer cmdBuf;

			switch(usage) {
				case CommandListUsage::Graphics:
				{
					info.commandPool = mGraphicsCommandPools[threadIndex];

					Lock lock(mGraphicsCommandPoolMutexes[threadIndex]);
					vkAllocateCommandBuffers(mDevice->GetHandle(), &info, &cmdBuf);
					lock.unlock();

					cmd = std::make_shared<CommandListVk>(*this, cmdBuf, usage, threadIndex, mGraphicsQueueFamilyIndex,
						isSub, debugName);
					break;
				}

				case CommandListUsage::TransferImmediate:
				{
					info.commandPool = mTransferImmediateCommandPools[threadIndex];

					Lock lock(mTransferImmediateCommandPoolMutexes[threadIndex]);
					vkAllocateCommandBuffers(mDevice->GetHandle(), &info, &cmdBuf);
					lock.unlock();

					cmd = std::make_shared<CommandListVk>(*this, cmdBuf, usage, threadIndex, mTransferImmediateQueueFamilyIndex,
						isSub, debugName);
					break;
				}

				case CommandListUsage::TransferDeferred:
				{
					info.commandPool = mTransferDeferredCommandPools[threadIndex];

					Lock lock(mTransferDeferredCommandPoolMutexes[threadIndex]);
					vkAllocateCommandBuffers(mDevice->GetHandle(), &info, &cmdBuf);
					lock.unlock();

					cmd = std::make_shared<CommandListVk>(*this, cmdBuf, usage, threadIndex, mTransferDeferredQueueFamilyIndex,
						isSub, debugName);
					break;
				}

				case CommandListUsage::Compute:
				{
					info.commandPool = mComputeCommandPools[threadIndex];

					Lock lock(mComputeCommandPoolMutexes[threadIndex]);
					vkAllocateCommandBuffers(mDevice->GetHandle(), &info, &cmdBuf);
					lock.unlock();

					cmd = std::make_shared<CommandListVk>(*this, cmdBuf, usage, threadIndex, mComputeQueueFamilyIndex,
						isSub, debugName);
					break;
				}

				default:
					ASSERTION_FAILED("Unknown CommandListUsage({0}).", (Uint32)usage);
			}

			return cmd;
		}

		void VulkanCommandListPool::Free(CommandListVk& commandList)
		{
			VkCommandBuffer cmdBuf = commandList.GetHandle();
			Uint32 index = commandList.GetAllocatedCommandPoolIndex();

			switch(commandList.GetUsage()) {
				case CommandListUsage::Graphics:
				{
					Lock lock(mGraphicsCommandPoolMutexes[index]);
					vkFreeCommandBuffers(mDevice->GetHandle(), mGraphicsCommandPools[index], 1, &cmdBuf);
					break;
				}

				case CommandListUsage::TransferImmediate:
				{
					Lock lock(mTransferImmediateCommandPoolMutexes[index]);
					vkFreeCommandBuffers(mDevice->GetHandle(), mTransferImmediateCommandPools[index], 1, &cmdBuf);
					break;
				}

				case CommandListUsage::TransferDeferred:
				{
					Lock lock(mTransferDeferredCommandPoolMutexes[index]);
					vkFreeCommandBuffers(mDevice->GetHandle(), mTransferDeferredCommandPools[index], 1, &cmdBuf);
					break;
				}

				case CommandListUsage::Compute:
				{
					Lock lock(mComputeCommandPoolMutexes[index]);
					vkFreeCommandBuffers(mDevice->GetHandle(), mComputeCommandPools[index], 1, &cmdBuf);
					break;
				}

				default:
					ASSERTION_FAILED("Unknown CommandListUsage({0}).", (Uint32)commandList.GetUsage());
			}
		}
	} // namespace render
} // namespace cube
