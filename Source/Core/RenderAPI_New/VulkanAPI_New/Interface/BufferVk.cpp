#include "BufferVk.h"

#include "../VulkanUtility.h"
#include "../VulkanDebug.h"
#include "DeviceVk.h"
#include "EngineCore/Assertion.h"

namespace cube
{
	namespace render
	{
		BufferVk::BufferVk(SPtr<DeviceVk> device, const BufferAttribute& attr) :
			mDevice(device)
		{
			VkResult res;

			VkBufferCreateInfo info;
			info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			info.pNext = nullptr;
			info.flags = 0;
			info.size = attr.size;
			info.queueFamilyIndexCount = 0;
			info.pQueueFamilyIndices = nullptr;
			info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

			info.usage = 0;
			if(attr.bindTypeFlags & BufferBindTypeFlagsBit::Vertex)
				info.usage |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
			if(attr.bindTypeFlags & BufferBindTypeFlagsBit::Index)
				info.usage |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
			if(attr.bindTypeFlags & BufferBindTypeFlagsBit::Uniform)
				info.usage |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
			if(attr.bindTypeFlags & BufferBindTypeFlagsBit::TransferSource)
				info.usage |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
			if(attr.bindTypeFlags & BufferBindTypeFlagsBit::TransferDest)
				info.usage |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;

			res = vkCreateBuffer(device->GetHandle(), &info, nullptr, &mBuffer);
			CheckVkResult("Failed to create buffer.", res);

			VulkanDebug::SetObjectName(device->GetHandle(), mBuffer, attr.debugName);
		}

		BufferVk::~BufferVk()
		{
			vkDestroyBuffer(mDevice->GetHandle(), mBuffer, nullptr);
		}

		void BufferVk::UpdateData(Uint64 offset, Uint64 size, const void* pData)
		{
		}

		void BufferVk::CopyData(const SPtr<Buffer>& src, Uint64 srcOffset, Uint64 dstOffset, Uint64 size)
		{
		}

		void BufferVk::Map(const void* pMappedData)
		{
		}

		void BufferVk::Unmap()
		{
		}

	} // namespace render
} // namespace cube
