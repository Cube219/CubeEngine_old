#pragma once

#include "../VulkanAPIHeader.h"

#include "BaseRenderAPI/Wrapper/BaseRenderBuffer.h"

namespace cube
{
	namespace core
	{
		class VULKAN_API_EXPORT VulkanBuffer : public BaseRenderBuffer
		{
		public:
			VulkanBuffer(const SPtr<VulkanDevice>& device, VkBufferUsageFlags usage,
				VkDeviceSize size, const void* data, VkSharingMode sharingMode);
			virtual ~VulkanBuffer();

			VkBuffer GetHandle() const { return mBuffer; }

			void Map() final override;
			void UpdateBufferData(const void* data, size_t size, uint64_t offset) final override;
			void Unmap() final override;

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

			SPtr<VulkanDevice> mDevice_ref;
		};
	}
}