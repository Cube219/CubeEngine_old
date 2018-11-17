#ifndef VULKAN_LOGICAL_DEVICE_H
#define VULKAN_LOGICAL_DEVICE_H

#include "VulkanAPIHeader.h"

#include "VkObject.h"

namespace cube
{
	namespace render
	{
		class VulkanLogicalDevice : public std::enable_shared_from_this<VulkanLogicalDevice>
		{
		public:
			VulkanLogicalDevice(VulkanPhysicalDevice& physicalDevice, const VkPhysicalDeviceFeatures& enabledFeatures,
				const DeviceAttribute& attr);
			~VulkanLogicalDevice();

			VkDevice GetHandle() const { return mDevice; }

			VulkanPhysicalDevice& GetParentPhysicalDevice() const { return mParentPhysicalDevice; }

			VkBufferWrapper CreateVkBufferWrapper(const VkBufferCreateInfo& info, const char* debugName = nullptr);
			VkFenceWrapper CreateVkFenceWrapper(const VkFenceCreateInfo& info, const char* debugName = nullptr);

			void ReleaseVkObject(VkBufferWrapper&& buffer) const;
			void ReleaseVkObject(VkFenceWrapper&& fence) const;

		private:
			VkDevice mDevice;

			VulkanPhysicalDevice& mParentPhysicalDevice;
		};
	} // namespace render
} // namespace cube

#endif // VULKAN_LOGICAL_DEVICE_H
