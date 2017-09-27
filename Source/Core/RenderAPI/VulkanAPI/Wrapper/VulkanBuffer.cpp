#include "VulkanBuffer.h"

#include <memory.h>

#include "VulkanDevice.h"

namespace cube
{
	namespace core
	{
		VulkanBuffer::VulkanBuffer(const SPtr<VulkanDevice>& device, VkBufferUsageFlags usage,
			VkDeviceSize size, const void* data, VkSharingMode sharingMode) :
			mDevice_ref(device)
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

			res = vkCreateBuffer(*device, &bufferCreateInfo, nullptr, &mBuffer);
			CheckVkResult(L"VulkanBuffer", L"Cannot create a VulkanBuffer", res);

			// Allocate memory
			VkMemoryRequirements memRequire;
			vkGetBufferMemoryRequirements(*device, mBuffer, &memRequire);

			mAllocatedMemory = device->AllocateMemory(memRequire,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

			// Mapping Setup
			res = vkMapMemory(*device, mAllocatedMemory, 0, memRequire.size, 0, &mMappedData);
			CheckVkResult(L"VulkanBuffer", L"Cannot map to the memory", res);
			mMappedSize = size;

			// Bind
			res = vkBindBufferMemory(*device, mBuffer, mAllocatedMemory, 0);
			CheckVkResult(L"VulkanBuffer", L"Cannot bind the memory", res);

			if(data != nullptr)
				UpdateBufferData(data, mMappedSize, 0);
		}

		VulkanBuffer::~VulkanBuffer()
		{
			vkUnmapMemory(*mDevice_ref, mAllocatedMemory);

			vkFreeMemory(*mDevice_ref, mAllocatedMemory, nullptr);
			vkDestroyBuffer(*mDevice_ref, mBuffer, nullptr);
		}

		void VulkanBuffer::UpdateBufferData(const void* data, size_t size, uint64_t offset)
		{
			char* p = (char*)mMappedData + offset;
			memcpy(p, data, size);
		}
	}
}