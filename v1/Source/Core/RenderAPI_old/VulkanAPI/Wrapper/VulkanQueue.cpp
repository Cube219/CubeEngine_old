#include "VulkanQueue.h"

namespace cube
{
	namespace render
	{
		VkQueueFlags GetVkQueueFlags(QueueTypeBits type)
		{
			VkQueueFlags f = 0;

			if(static_cast<int>(type & QueueTypeBits::GraphicsBit) > 0)
				f |= VK_QUEUE_GRAPHICS_BIT;

			if(static_cast<int>(type & QueueTypeBits::ComputeBit) > 0)
				f |= VK_QUEUE_COMPUTE_BIT;

			if(static_cast<int>(type & QueueTypeBits::TransferBit) > 0)
				f |= VK_QUEUE_TRANSFER_BIT;

			if(static_cast<int>(type & QueueTypeBits::SparseBindingBit) > 0)
				f |= VK_QUEUE_SPARSE_BINDING_BIT;

			return f;
		}

		VulkanQueue::VulkanQueue(VkQueue queue, VkQueueFlags types, uint32_t index) :
			mQueue(queue)
		{
			mTypes = SCast(QueueTypeBits)(0);
			if((types & VK_QUEUE_GRAPHICS_BIT) > 0)
				mTypes |= QueueTypeBits::GraphicsBit;
			if((types & VK_QUEUE_COMPUTE_BIT) > 0)
				mTypes |= QueueTypeBits::ComputeBit;
			if((types & VK_QUEUE_TRANSFER_BIT) > 0)
				mTypes |= QueueTypeBits::TransferBit;
			if((types & VK_QUEUE_SPARSE_BINDING_BIT) > 0)
				mTypes |= QueueTypeBits::SparseBindingBit;

			mIndex = index;
		}

		VulkanQueue::~VulkanQueue()
		{
		}
	} // namespace render
} // namespace cube
