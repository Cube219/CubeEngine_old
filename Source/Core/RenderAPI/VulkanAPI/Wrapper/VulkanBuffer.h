#pragma once

#include "../VulkanAPIHeader.h"

#include "BaseRenderAPI/Wrapper/BaseRenderBuffer.h"

namespace cube
{	
	namespace core
	{
		VkBufferUsageFlags GetVkBufferUsageFlags(BufferTypeBits typeBits);

		class VULKAN_API_EXPORT VulkanBuffer : public BaseRenderBuffer, public std::enable_shared_from_this<VulkanBuffer>
		{
		public:
			VulkanBuffer(const SPtr<VulkanDevice>& device, BaseRenderBufferInitializer& initializer);
			virtual ~VulkanBuffer();

			VkBuffer GetHandle() const { return mBuffer; }

			void Map() final override;
			void UpdateBufferData(uint64_t index, const void* data, size_t size) final override;
			void Unmap() final override;

			BaseRenderBufferInfo GetInfo(uint64_t index) const final override;

			const VkDescriptorBufferInfo GetVulkanInfo(uint64_t offset, uint64_t range) const
			{
				VkDescriptorBufferInfo info;
				info.buffer = mBuffer;
				info.offset = offset;
				info.range = range;

				return info;
			}

		private:
			VkBuffer mBuffer;
			VkDeviceMemory mAllocatedMemory;

			void* mMappedData;
			VkDeviceSize mMappedSize;
			Vector<uint64_t> mDataOffsets;
			Vector<uint64_t> mDataSizes;

			SPtr<VulkanDevice> mDevice_ref;
		};
	}
}