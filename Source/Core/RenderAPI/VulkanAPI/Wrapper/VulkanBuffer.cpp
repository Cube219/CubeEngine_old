#include "VulkanBuffer.h"

#include <memory.h>

#include "VulkanDevice.h"

namespace cube
{
	namespace render
	{
		VkBufferUsageFlags GetVkBufferUsageFlags(BufferTypeBits typeBits)
		{
			VkBufferUsageFlags f = 0;

			if(SCast(int)(typeBits | BufferTypeBits::Uniform) > 0)
				f |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
			if(SCast(int)(typeBits | BufferTypeBits::Vertex) > 0)
				f |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
			if(SCast(int)(typeBits | BufferTypeBits::Index) > 0)
				f |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
			if(SCast(int)(typeBits | BufferTypeBits::TransferSource) > 0)
				f |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

			return f;
		}

		VulkanBuffer::VulkanBuffer(const SPtr<VulkanDevice>& device, BufferInitializer& initializer) : 
			mDevice_ref(device), mMappedData(nullptr)
		{
			VkResult res;

			// Get total of aligned data size 
			uint64_t totalDataSize = 0;
			uint64_t align = device->GetProperties().limits.minUniformBufferOffsetAlignment;

			uint64_t bufDataNum = initializer.bufferDatas.size();
			mDataOffsets.resize(bufDataNum);
			mDataSizes.resize(bufDataNum);

			for(uint64_t i = 0; i < bufDataNum; i++) {
				mDataOffsets[i] = totalDataSize;
				mDataSizes[i] = initializer.bufferDatas[i].size;

				uint64_t alignedSize = initializer.bufferDatas[i].size + align - 1;
				alignedSize /= align;
				alignedSize *= align;
				totalDataSize += alignedSize;
			}

			mSize = totalDataSize;

			// Create buffer
			VkBufferCreateInfo bufferCreateInfo = {};
			bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			bufferCreateInfo.pNext = nullptr;
			bufferCreateInfo.flags = 0;
			bufferCreateInfo.usage = GetVkBufferUsageFlags(initializer.type);
			bufferCreateInfo.size = totalDataSize;
			bufferCreateInfo.queueFamilyIndexCount = 0;
			bufferCreateInfo.pQueueFamilyIndices = nullptr;
			bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

			res = vkCreateBuffer(device->GetHandle(), &bufferCreateInfo, nullptr, &mBuffer);
			CheckVkResult("Cannot create a VulkanBuffer", res);

			// Allocate memory
			VkMemoryRequirements memRequire;
			vkGetBufferMemoryRequirements(device->GetHandle(), mBuffer, &memRequire);

			mAllocatedMemory = device->AllocateMemory(memRequire,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

			mMappedSize = memRequire.size;

			// Bind
			res = vkBindBufferMemory(device->GetHandle(), mBuffer, mAllocatedMemory, 0);
			CheckVkResult("Cannot bind the memory", res);

			// Update buffer data
			for(uint64_t i = 0; i < bufDataNum; i++) {
				if(initializer.bufferDatas[i].data != nullptr) {
					Map();
					UpdateBufferData(i, initializer.bufferDatas[i].data, initializer.bufferDatas[i].size);
				}
			}
			Unmap();
		}

		VulkanBuffer::~VulkanBuffer()
		{
			if(mMappedData != nullptr)
				vkUnmapMemory(mDevice_ref->GetHandle(), mAllocatedMemory);

			vkFreeMemory(mDevice_ref->GetHandle(), mAllocatedMemory, nullptr);
			vkDestroyBuffer(mDevice_ref->GetHandle(), mBuffer, nullptr);
		}

		void VulkanBuffer::Map()
		{
			if(mMappedData != nullptr)
				return;

			VkResult res;

			res = vkMapMemory(mDevice_ref->GetHandle(), mAllocatedMemory, 0, mMappedSize, 0, &mMappedData);
			CheckVkResult("Cannot map to the memory", res);

			mMappedOffset = 0;
		}

		void VulkanBuffer::Map(uint64_t startIndex, uint64_t endIndex)
		{
			if(mMappedData != nullptr)
				return;

			VkResult res;

			uint64_t startOffset = mDataOffsets[startIndex];
			uint64_t size = mDataOffsets[endIndex] + mDataSizes[endIndex] - startOffset;

			res = vkMapMemory(mDevice_ref->GetHandle(), mAllocatedMemory, startOffset, size, 0, &mMappedData);
			CheckVkResult("Cannot map to the memory", res);

			mMappedOffset = startOffset;
		}

		void VulkanBuffer::UpdateBufferData(uint64_t index, const void* data, size_t size)
		{
#ifdef _DEBUG
			if(size != mDataSizes[index]) {
				CUBE_LOG(cube::LogType::Error, "Wrong data size. (Expected: {0} / Actual: {1})", mDataSizes[index], size);
				return;
			}
#endif // _DEBUG

			if(mMappedData == nullptr) {
				CUBE_LOG(cube::LogType::Error, "Cannot update buffer data. It is not mapped.");
				return;
			}

			char* p = (char*)mMappedData + mDataOffsets[index] - mMappedOffset;
			memcpy(p, data, size);
		}

		void VulkanBuffer::Unmap()
		{
			if(mMappedData == nullptr)
				return;

			vkUnmapMemory(mDevice_ref->GetHandle(), mAllocatedMemory);
			mMappedData = nullptr;
		}

		BufferInfo VulkanBuffer::GetInfo(uint64_t index) const
		{
			BufferInfo info;
			info.buffer = shared_from_this();
			info.offset = mDataOffsets[index];
			info.range = mDataSizes[index];

			return info;
		}
	} // namespace render
} // namespace cube
