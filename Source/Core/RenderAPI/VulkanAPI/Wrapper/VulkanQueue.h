#pragma once

#include "..\VulkanAPIHeader.h"

#include "BaseRenderAPI\Wrapper\BaseRenderQueue.h"

namespace cube
{
	namespace core
	{
		struct VulkanQueueFamily; // Defined at VulkanAPIHeader.h

		// --------------------------------------------
		//                 VulkanQueue
		// --------------------------------------------

		VkQueueFlags GetVkQueueFlags(QueueTypeBits type);

		class VULKAN_API_EXPORT VulkanQueue : public BaseRenderQueue
		{
			friend VulkanDevice;

		public:
			~VulkanQueue();

			operator VkQueue() const
			{
				return mQueue;
			}

		private:
			VulkanQueue(){ }
			VulkanQueue(VkQueue queue);

			VkQueue mQueue;
		};
	}
}