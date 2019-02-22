#pragma once

#include "VulkanAPIHeader.h"

#include "RenderAPI/RenderAPI.h"

namespace cube
{
	namespace render
	{
		extern "C" VULKAN_API_EXPORT RenderAPI* GetAPI();

		class VULKAN_API_EXPORT VulkanAPI final : public RenderAPI
		{
		public:
			VulkanAPI() {}
			virtual ~VulkanAPI();

			virtual void Init(const RenderAPIAttribute& attr) override final;

			virtual SPtr<Device> GetDevice(const DeviceAttribute& attr) override final;

		private:
			void CreateInstance(const RenderAPIAttribute& attr);

			VkInstance mInstance;
			Vector<VulkanPhysicalDevice> mPhysicalDevices;
		};
	} // namespace render
} // namespace cube
