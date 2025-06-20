#pragma once

#include "../VulkanAPIHeader.h"

#include "BaseRenderAPI/Wrapper/Buffer.h"

namespace cube
{	
	namespace render
	{
		VkBufferUsageFlags GetVkBufferUsageFlags(BufferTypeFlags typeBits);

		class VULKAN_API_EXPORT VulkanBuffer : public Buffer, public std::enable_shared_from_this<VulkanBuffer>
		{
		public:
			VulkanBuffer(const SPtr<VulkanDevice>& device, BufferInitializer& initializer);
			virtual ~VulkanBuffer();

			VkBuffer GetHandle() const { return mBuffer; }

			void Map() final override;
			void Map(uint64_t startIndex, uint64_t endIndex) final override;
			void UpdateBufferData(uint64_t index, const void* data, size_t size) final override;
			void Unmap() final override;

			BufferInfo GetInfo(uint64_t index) const final override;

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
			uint64_t mMappedOffset;
			Vector<uint64_t> mDataOffsets;
			Vector<uint64_t> mDataSizes;

			SPtr<VulkanDevice> mDevice_ref;
		};
	} // namespace render
} // namespace cube
