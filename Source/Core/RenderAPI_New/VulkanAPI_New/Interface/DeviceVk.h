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
			DeviceVk(SPtr<VulkanPhysicalDevice>& physicalDevice, const VkPhysicalDeviceFeatures& enabledFeatures,
				const DeviceAttribute& attr);
			virtual ~DeviceVk();

			VkDevice GetHandle() const { return mDevice; }

			virtual SPtr<Buffer> CreateBuffer(const BufferAttribute& attr) override final;
			// CreateShader
			// CreateTexture
			// CreateSampler
			// CreatePipelineState
			// CreateFence

			SPtr<VulkanPhysicalDevice> GetParentPhysicalDevice() const { return mPhysicalDevice; }

		private:
			VkDeviceMemory AllocateMemory(VkMemoryRequirements requirements, VkMemoryPropertyFlags properties);

			VkDevice mDevice;

			SPtr<VulkanPhysicalDevice> mPhysicalDevice;
		};
	} // namespace render
} // namespace cube
