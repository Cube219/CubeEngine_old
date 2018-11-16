#pragma once

#include "../VulkanAPIHeader.h"

#include "BaseRenderAPI_New/Interface/Device.h"

#include "../VulkanMemoryManager.h"
#include "../VulkanCommandListPool.h"
#include "../VulkanQueueManager.h"
#include "../VulkanFencePool.h"
#include "../VulkanSemaphorePool.h"
#include "../VulkanLogicalDevice.h"

#include "EngineCore/Thread/MutexLock.h"

namespace cube
{
	namespace render
	{
		class DeviceVk final : public Device, public std::enable_shared_from_this<DeviceVk>
		{
		public:
			DeviceVk(SPtr<VulkanLogicalDevice> device);
			virtual ~DeviceVk();

			virtual SPtr<Buffer> CreateBuffer(const BufferAttribute& attr) override final;

			virtual SPtr<CommandList> GetCommandList(CommandListUsage usage) override final;
			// CreateShader
			// CreateTexture
			// CreateSampler
			// CreatePipelineState
			// CreateFence

			virtual SPtr<Fence> SubmitCommandList(SPtr<CommandList>& commandList) override final;

			virtual void FinishFrame() override final;

			SPtr<VulkanLogicalDevice> GetLogicalDevice() const { return mDevice; }
			VulkanMemoryManager& GetMemoryManager() { return mMemoryManager; }
			VulkanQueueManager& GetQueueManager() { return mQueueManager; }
			
			template <typename VkObjectType>
			void ReleaseAtEndOfFrame(VkObjectType&& vkObject)
			{
				core::Lock lock(mReleaseQueueMutex);

				mReleaseVkObjectQueue.push_back(VkObjectStorage::Create(std::move(vkObject)));
			}

			template <typename T>
			void ReleaseAtEndOfFrame(SPtr<T> ptrObject)
			{
				core::Lock lock(mReleaseQueueMutex);

				mReleaseFuncQueue.push_back([ptrObject]() mutable {
					ptrObject.reset();
				});
			}

		private:
			SPtr<VulkanLogicalDevice> mDevice;

			// VulkanPhysicalDevice& mParentPhysicalDevice;
			VulkanMemoryManager mMemoryManager;

			VulkanCommandListPool mCommandListPool;
			VulkanQueueManager mQueueManager;
			VulkanFencePool mFencePool;
			VulkanSemaphorePool mSemaphorePool;

			core::Mutex mReleaseQueueMutex;
			Vector<std::function<void()>> mReleaseFuncQueue;
			Vector<VkObjectStorage> mReleaseVkObjectQueue;
		};
	} // namespace render
} // namespace cube
