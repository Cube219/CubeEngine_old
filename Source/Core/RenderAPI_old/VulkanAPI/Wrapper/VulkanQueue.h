#pragma once

#include "../VulkanAPIHeader.h"

#include "BaseRenderAPI/Wrapper/Queue.h"

namespace cube
{
	namespace render
	{
		struct VulkanQueueFamily; // Defined at VulkanAPIHeader.h

		// --------------------------------------------
		//                 VulkanQueue
		// --------------------------------------------

		VkQueueFlags GetVkQueueFlags(QueueTypeBits type);

		class VULKAN_API_EXPORT VulkanQueue : public Queue
		{
			friend VulkanDevice;

		public:
			~VulkanQueue();

			VkQueue GetHandle() const { return mQueue; }

		private:
			VulkanQueue(){ }
			VulkanQueue(VkQueue queue, VkQueueFlags types, uint32_t index);

			VkQueue mQueue;
		};
	} // namespace render
} // namespace cube
