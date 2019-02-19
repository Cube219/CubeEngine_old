#pragma once

#include "../VulkanAPIHeader.h"

#include "RenderAPI/Interface/Buffer.h"

#include "../VkObject.h"
#include "../VulkanMemoryManager.h"

namespace cube
{
	namespace render
	{
		class BufferVk final : public Buffer
		{
		public:
			BufferVk(DeviceVk& device, const BufferAttribute& attr,
				VulkanQueueManager& queueManager, VulkanCommandListPool& cmdListPool);
			virtual ~BufferVk();

			VkBuffer GetHandle() const { return mBuffer.mObject; }

			virtual void UpdateData(CommandList& cmdList, Uint64 offset, Uint64 size, const void* pData) override final;
			virtual void CopyData(CommandList& cmdList, const Buffer& src, Uint64 srcOffset,
				Uint64 dstOffset, Uint64 size) override final;

			virtual void Map(void*& pMappedData) override final;
			virtual void Unmap() override final;

			virtual void CreateView() override final;

		private:
			VkBufferWrapper mBuffer;

			VulkanAllocation mMemoryAllocation;
		};
	} // namespace render
} // namespace cube
