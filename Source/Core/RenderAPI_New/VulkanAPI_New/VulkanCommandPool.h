#pragma once

#include "VulkanAPIHeader.h"

namespace cube
{
	namespace render
	{
		class VulkanCommandPool
		{
		public:
			VulkanCommandPool(const SPtr<DeviceVk>& device);
			~VulkanCommandPool();

		private:
			VkCommandPool mPool;
		};
	} // namespace render
} // namespace cube
