#include "BufferVk.h"

#include "../VulkanUtility.h"
#include "../VulkanDebug.h"
#include "../VulkanMemoryManager.h"
#include "../VulkanQueueManager.h"
#include "../VulkanCommandListPool.h"
#include "../VulkanLogicalDevice.h"
#include "DeviceVk.h"
#include "CommandListVk.h"
#include "EngineCore/Assertion.h"

namespace cube
{
	namespace render
	{
		BufferVk::BufferVk(DeviceVk& device, const BufferAttribute& attr,
			VulkanQueueManager& queueManager, VulkanCommandListPool& cmdListPool) :
			Buffer(attr)
		{
			// Check buffer attr
#ifdef _DEBUG
			if(attr.cpuAccessible == true) {
				CHECK(attr.usage == ResourceUsage::Dynamic || attr.usage == ResourceUsage::Staging,
					"The buffer must be dynamic or staging if it is accessible by CPU.");
			}
			CHECK(attr.usage != ResourceUsage::Immutable || attr.pData != nullptr,
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
			if(attr.bindTypeFlags.IsSet(BufferBindTypeFlag::Vertex))
				info.usage |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
			if(attr.bindTypeFlags.IsSet(BufferBindTypeFlag::Index))
				info.usage |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
			if(attr.bindTypeFlags.IsSet(BufferBindTypeFlag::Uniform))
				info.usage |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
			if(attr.bindTypeFlags.IsSet(BufferBindTypeFlag::TransferSource))
				info.usage |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
			if(attr.bindTypeFlags.IsSet(BufferBindTypeFlag::TransferDest))
				info.usage |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
			// If the usage is default or immutable, the buffer will be transferred from a staging buffer
			if(attr.usage == ResourceUsage::Default || attr.usage == ResourceUsage::Immutable)
				info.usage |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;

			mBuffer = device.GetLogicalDevice()->CreateVkBufferWrapper(info, attr.debugName);

			// Allocate memory
			MemoryUsage memUsage;
			switch(attr.usage) {
				case ResourceUsage::Default:
				case ResourceUsage::Immutable:
					memUsage = MemoryUsage::GPU;
					break;
					
				case ResourceUsage::Dynamic:
					memUsage = MemoryUsage::CPUtoGPU;
					break;

				case ResourceUsage::Staging:
					memUsage = MemoryUsage::CPU;
					break;
			}

			if(attr.isDedicated) {
				VkBufferMemoryRequirementsInfo2 memRequireInfo;
				memRequireInfo.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_REQUIREMENTS_INFO_2;
				memRequireInfo.pNext = nullptr;
				memRequireInfo.buffer = mBuffer.mObject;

				VkMemoryDedicatedRequirements memDediRequire;
				memDediRequire.sType = VK_STRUCTURE_TYPE_MEMORY_DEDICATED_REQUIREMENTS;
				memDediRequire.pNext = nullptr;
				memDediRequire.prefersDedicatedAllocation = VK_TRUE;
				memDediRequire.requiresDedicatedAllocation = VK_TRUE;

				VkMemoryRequirements2 memRequire;
				memRequire.sType = VK_STRUCTURE_TYPE_MEMORY_REQUIREMENTS_2;
				memRequire.pNext = &memDediRequire;

				vkGetBufferMemoryRequirements2(mBuffer.GetVkDevice(), &memRequireInfo, &memRequire);

				VkMemoryDedicatedAllocateInfo dediAllocInfo;
				dediAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_DEDICATED_ALLOCATE_INFO;
				dediAllocInfo.pNext = nullptr;
				dediAllocInfo.buffer = mBuffer.mObject;
				dediAllocInfo.image = VK_NULL_HANDLE;

				mMemoryAllocation = device.GetMemoryManager().AllocateDedicated(memRequire, dediAllocInfo, memUsage);
			} else {
				VkMemoryRequirements memRequire;
				vkGetBufferMemoryRequirements(mBuffer.GetVkDevice(), mBuffer.mObject, &memRequire);

				mMemoryAllocation = device.GetMemoryManager().Allocate(memRequire, memUsage);
			}

			// Bind memory
			res = vkBindBufferMemory(mBuffer.GetVkDevice(), mBuffer.mObject,
				mMemoryAllocation.deviceMemory, mMemoryAllocation.offset);
			CHECK_VK(res, "Failed to bind buffer memory.");

			// Initialize buffer data if it is existed
			if(attr.pData != nullptr && attr.size > 0) {
				if(attr.usage == ResourceUsage::Dynamic || attr.usage == ResourceUsage::Staging) {
					void* mappedPtr;
					Map(mappedPtr);
					memcpy(mappedPtr, attr.pData, attr.size);
					Unmap();
				} else {
					VkBufferCreateInfo stagingBufferInfo = info;
					stagingBufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

					U8String stagingBufDebugName = fmt::format("Staging buffer for '{0}'", attr.debugName);

					VkBufferWrapper stagingBuffer =
						device.GetLogicalDevice()->CreateVkBufferWrapper(stagingBufferInfo, stagingBufDebugName.c_str());

					VkMemoryRequirements stagingMemRequire;
					vkGetBufferMemoryRequirements(stagingBuffer.GetVkDevice(), stagingBuffer.mObject, &stagingMemRequire);
					VulkanAllocation stagingAlloc = device.GetMemoryManager().Allocate(stagingMemRequire, MemoryUsage::CPU);

					void* stagingData = stagingAlloc.mappedData;
					memcpy(stagingData, attr.pData, attr.size);

					res = vkBindBufferMemory(stagingBuffer.GetVkDevice(), stagingBuffer.mObject,
						stagingAlloc.pPage->GetVkDeviceMemory(), stagingAlloc.offset);
					CHECK_VK(res, "Failed to bind staging buffer memory.");

					CommandListAttribute cmdListAttr;
					cmdListAttr.usage = CommandListUsage::TransferImmediate;
					cmdListAttr.threadIndex = 0;
					cmdListAttr.isSub = false;
					cmdListAttr.debugName = "Command List to transfer buffer data";
					SPtr<CommandListVk> cmdBuf = cmdListPool.Allocate(cmdListAttr);
					cmdBuf->Begin();
					VkCommandBuffer vkCmdBuf = cmdBuf->GetHandle();

					VkBufferMemoryBarrier memBarrier;
					memBarrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
					memBarrier.pNext = nullptr;
					memBarrier.srcAccessMask = 0; // NONE
					memBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
					memBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
					memBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
					memBarrier.buffer = mBuffer.mObject;
					memBarrier.offset = 0;
					memBarrier.size = attr.size;
					vkCmdPipelineBarrier(vkCmdBuf, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
						0, nullptr, 1, &memBarrier, 0, nullptr);

					VkBufferCopy bufCopy;
					bufCopy.srcOffset = 0;
					bufCopy.dstOffset = 0;
					bufCopy.size = attr.size;
					vkCmdCopyBuffer(vkCmdBuf, stagingBuffer.mObject, mBuffer.mObject, 1, &bufCopy);
					cmdBuf->End();

					queueManager.SubmitCommandList(*cmdBuf);

					device.ReleaseAtNextFrame(cmdBuf);
					device.ReleaseAtNextFrame(std::move(stagingAlloc));
					device.ReleaseAtNextFrame(std::move(stagingBuffer));
				}
			}
		}

		BufferVk::~BufferVk()
		{
			mMemoryAllocation.Free();
			mBuffer.Release();
		}

		void BufferVk::UpdateData(CommandList& cmdList, Uint64 offset, Uint64 size, const void* pData)
		{
			CHECK(mUsage != ResourceUsage::Immutable, "Immutable buffer cannot be updated.");
			CHECK(mUsage != ResourceUsage::Dynamic, "Dynamic buffer should be updated via map.");
			// TODO: UpdateData 구현
		}

		void BufferVk::CopyData(CommandList& cmdList, const Buffer& src, Uint64 srcOffset,
			Uint64 dstOffset, Uint64 size)
		{
			CHECK(mUsage != ResourceUsage::Immutable, "Immutable buffer cannot be updated.");
			
			cmdList.CopyBuffer(src, *this, srcOffset, dstOffset, size);
		}

		void BufferVk::Map(void*& pMappedData)
		{
			CHECK(mUsage == ResourceUsage::Dynamic || mUsage == ResourceUsage::Staging,
				"Only the dynamic or staging buffer can be mapped.");

			pMappedData = mMemoryAllocation.mappedData;
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
