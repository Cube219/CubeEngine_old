#pragma once

#include "../VulkanAPIHeader.h"

#include "BaseRenderAPI_New/Interface/Device.h"

namespace cube
{
	namespace render
	{
		class DeviceVk final : public Device, public std::enable_shared_from_this<DeviceVk>
		{
		public:
			DeviceVk(VulkanPhysicalDevice& physicalDevice, const VkPhysicalDeviceFeatures& enabledFeatures,
				const DeviceAttribute& attr);
			virtual ~DeviceVk();

			VkDevice GetHandle() const { return mDevice; }

			virtual SPtr<Buffer> CreateBuffer(const BufferAttribute& attr) override final;
			// CreateShader
			// CreateTexture
			// CreateSampler
			// CreatePipelineState
			// CreateFence

			virtual void SubmitCommandList(SPtr<CommandList>& commandList) override final;

			VulkanPhysicalDevice& GetParentPhysicalDevice() const { return mParentPhysicalDevice; }
			UPtr<VulkanMemoryManager>& GetMemoryManager() { return mMemoryManager; }
			UPtr<VulkanQueueManager>& GetQueueManager() { return mQueueManager; }

			Uint32 GetGraphicsQueueFamilyIndex() const { return mGraphicsQueueFamilyIndex; }
			Uint32 GetTransferQueueFamilyIndex() const { return mTransferQueueFamilyIndex; }

		private:
			VkDevice mDevice;

			VulkanPhysicalDevice& mParentPhysicalDevice;
			UPtr<VulkanMemoryManager> mMemoryManager;
			UPtr<VulkanQueueManager> mQueueManager;
			UPtr<VulkanFencePool> mFencePool;
			UPtr<VulkanSemaphorePool> mSemaphorePool;

			Uint32 mGraphicsQueueFamilyIndex;
			Uint32 mTransferQueueFamilyIndex;
		};
	} // namespace render
} // namespace cube
