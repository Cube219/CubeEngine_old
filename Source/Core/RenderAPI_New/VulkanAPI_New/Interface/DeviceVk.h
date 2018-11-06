#pragma once

#include "../VulkanAPIHeader.h"

#include "BaseRenderAPI_New/Interface/Device.h"

#include "../VulkanMemoryManager.h"

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

			VulkanPhysicalDevice& GetParentPhysicalDevice() const { return mParentPhysicalDevice; }
			UPtr<VulkanMemoryManager>& GetMemoryManager() { return mMemoryManager; }

		private:
			VkDevice mDevice;

			VulkanPhysicalDevice& mParentPhysicalDevice;
			UPtr<VulkanMemoryManager> mMemoryManager;
		};
	} // namespace render
} // namespace cube
