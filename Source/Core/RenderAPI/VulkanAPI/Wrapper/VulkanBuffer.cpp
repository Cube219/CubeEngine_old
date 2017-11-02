#include "VulkanBuffer.h"

#include <memory.h>

#include "VulkanDevice.h"

namespace cube
{
	namespace core
	{
		VulkanBuffer::VulkanBuffer(const SPtr<VulkanDevice>& device, VkBufferUsageFlags usage,
			VkDeviceSize size, const void* data, VkSharingMode sharingMode) :
			mDevice_ref(device), mMappedData(nullptr)
		{
			VkResult res;

			// Create buffer
			VkBufferCreateInfo bufferCreateInfo = {};
			bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			bufferCreateInfo.pNext = nullptr;
			bufferCreateInfo.flags = 0;
			bufferCreateInfo.usage = usage;
			bufferCreateInfo.size = size;
			bufferCreateInfo.queueFamilyIndexCount = 0;
			bufferCreateInfo.pQueueFamilyIndices = nullptr;
			bufferCreateInfo.sharingMode = sharingMode;

			res = vkCreateBuffer(device->GetHandle(), &bufferCreateInfo, nullptr, &mBuffer);
			CheckVkResult(L"VulkanBuffer", L"Cannot create a VulkanBuffer", res);

			// Allocate memory
			VkMemoryRequirements memRequire;
			vkGetBufferMemoryRequirements(device->GetHandle(), mBuffer, &memRequire);

			mAllocatedMemory = device->AllocateMemory(memRequire,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

			mMappedSize = memRequire.size;

			// Bind
			res = vkBindBufferMemory(device->GetHandle(), mBuffer, mAllocatedMemory, 0);
			CheckVkResult(L"VulkanBuffer", L"Cannot bind the memory", res);

			if(data != nullptr) {
				Map();
				UpdateBufferData(data, mMappedSize, 0);
				Unmap();
			}
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
			CheckVkResult(L"VulkanBuffer", L"Cannot map to the memory", res);
		}

		void VulkanBuffer::UpdateBufferData(const void* data, size_t size, uint64_t offset)
		{
			if(mMappedData == nullptr) {
				PrintlnLogWithSayer(L"VulkanBuffer", L"Cannot update buffer data. It is not mapped.");
				return;
			}

			char* p = (char*)mMappedData + offset;
			memcpy(p, data, size);
		}

		void VulkanBuffer::Unmap()
		{
			vkUnmapMemory(mDevice_ref->GetHandle(), mAllocatedMemory);
			mMappedData = nullptr;
		}
	}
}