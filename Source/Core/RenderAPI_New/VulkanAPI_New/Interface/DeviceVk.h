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
			DeviceVk(UPtr<VulkanPhysicalDevice>& physicalDevice, const VkPhysicalDeviceFeatures& enabledFeatures,
				bool enableDebugLayer);
			virtual ~DeviceVk();

			VkDevice GetHandle() const { return mDevice; }

			// CreateBuffer
			// CreateShader
			// CreateTexture
			// CreateSampler
			// CreatePipelineState
			// CreateFence

		private:
			VkDeviceMemory AllocateMemory(VkMemoryRequirements requirements, VkMemoryPropertyFlags properties);

			VkDevice mDevice;
		};
	} // namespace render
} // namespace cube
