#include "BufferVk.h"

#include "../VulkanUtility.h"
#include "../VulkanDebug.h"
#include "../VulkanMemoryManager.h"
#include "DeviceVk.h"
#include "EngineCore/Assertion.h"

namespace cube
{
	namespace render
	{
		BufferVk::BufferVk(SPtr<DeviceVk> device, const BufferAttribute& attr) :
			mDevice(device)
		{
			mUsage = attr.usage;

			// Check buffer attr
#ifdef _DEBUG
			if(attr.cpuAccessible == true) {
				CHECK(attr.usage == BufferUsage::Dynamic || attr.usage == BufferUsage::Staging,
					"The buffer must be dynamic or staging if it is accessible by CPU.");
			}
			CHECK(attr.usage != BufferUsage::Immutable || attr.pData != nullptr,
				"The immutable buffer must be initialized with data at creation time.");
#endif // _DEBUG

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

			// Allocate memory
			MemoryUsage memUsage;
			switch(attr.usage) {
				case BufferUsage::Default:
				case BufferUsage::Immutable:
					memUsage = MemoryUsage::GPU;
					break;
					
				case BufferUsage::Dynamic:
					memUsage = MemoryUsage::CPUtoGPU;
					break;

				case BufferUsage::Staging:
					memUsage = MemoryUsage::CPU;
					break;
			}

			VkMemoryRequirements memRequire;
			vkGetBufferMemoryRequirements(device->GetHandle(), mBuffer, &memRequire);

			mMemoryAllocation = device->GetMemoryManager()->Allocate(memRequire, memUsage);

			// Bind memory
			VkDeviceMemory deviceMem = mMemoryAllocation.pPage->GetVkDeviceMemory();
			res = vkBindBufferMemory(device->GetHandle(), mBuffer, deviceMem, mMemoryAllocation.offset);
			CheckVkResult("Failed to bind buffer memory.", res);

			// Initialize buffer data if it is existed
			if(attr.pData != nullptr && attr.size > 0) {
				if(attr.usage == BufferUsage::Dynamic || attr.usage == BufferUsage::Staging) {
					void* mappedPtr;
					Map(mappedPtr);
					memcpy(mappedPtr, attr.pData, attr.size);
					Unmap();
				} else {
					// TODO: Buffer Copy 구현
					//       CommandPool / CommandBuffer 구현 후
					VkBufferCreateInfo stagingBufferInfo = info;
					stagingBufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
				}
			}
		}

		BufferVk::~BufferVk()
		{
			mMemoryAllocation.pPage->Free(mMemoryAllocation);
			vkDestroyBuffer(mDevice->GetHandle(), mBuffer, nullptr);
		}

		void BufferVk::UpdateData(Uint64 offset, Uint64 size, const void* pData)
		{
			CHECK(mUsage != BufferUsage::Immutable, "Immutable buffer cannot be updated.");
			// TODO: UpdateData 구현
		}

		void BufferVk::CopyData(const SPtr<Buffer>& src, Uint64 srcOffset, Uint64 dstOffset, Uint64 size)
		{
			CHECK(mUsage != BufferUsage::Immutable, "Immutable buffer cannot be updated.");
			// TODO: CopyData 구현
		}

		void BufferVk::Map(void*& pMappedData)
		{
			CHECK(mUsage == BufferUsage::Dynamic || mUsage == BufferUsage::Staging,
				"Only the dynamic or staging buffer can be mapped.");

			// TODO: Map 구현
		}

		void BufferVk::Unmap()
		{
			// TODO: Unmap 구현
		}

		void BufferVk::CreateView()
		{
		}

	} // namespace render
} // namespace cube
