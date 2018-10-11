#pragma once

#include "../VulkanAPIHeader.h"

#include "BaseRenderAPI_New/Interface/Device.h"

namespace cube
{
	namespace render
	{
		class DeviceVk final : public Device
		{
		public:
			DeviceVk(SPtr<VulkanPhysicalDevice>& physicalDevice, const VkPhysicalDeviceFeatures& enabledFeatures,
				const DeviceAttribute& attr);
			virtual ~DeviceVk();

			VkDevice GetHandle() const { return mDevice; }

			virtual SPtr<Buffer> CreateBuffer(const BufferAttribute& attr) override final;
			// CreateBuffer
			// CreateShader
			// CreateTexture
			// CreateSampler
			// CreatePipelineState
			// CreateFence

		private:
			VkDeviceMemory AllocateMemory(VkMemoryRequirements requirements, VkMemoryPropertyFlags properties);

			VkDevice mDevice;

			SPtr<VulkanPhysicalDevice> mPhysicalDevice;
		};
	} // namespace render
} // namespace cube
