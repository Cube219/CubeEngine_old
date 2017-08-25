#pragma once

#include "..\VulkanAPIHeader.h"

#include "BaseRenderAPI\Wrapper\BaseRenderBuffer.h"

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

			operator VkBuffer() const
			{
				return mBuffer;
			}

			void UpdateBufferData(const void* data, size_t size, uint64_t offset) override;

			const VkDescriptorBufferInfo GetVulkanInfo(uint64_t offset, uint64_t range) const;

		private:
			VkBuffer mBuffer;
			VkDeviceMemory mAllocatedMemory;

			void* mMappedData;
			VkDeviceSize mMappedSize;

			SPtr<VulkanDevice> mDevice_ref;
		};

		inline const VkDescriptorBufferInfo VulkanBuffer::GetVulkanInfo(uint64_t offset, uint64_t range) const
		{
			VkDescriptorBufferInfo info;
			info.buffer = mBuffer;
			info.offset = offset;
			info.range = range;

			return info;
		}
	}
}