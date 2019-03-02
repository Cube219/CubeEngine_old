#include "VulkanMemoryManager.h"

#include "VulkanUtility.h"
#include "VulkanDebug.h"
#include "Interface/DeviceVk.h"
#include "VulkanPhysicalDevice.h"
#include "VulkanLogicalDevice.h"
#include "Base/Math.h"

namespace cube
{
	namespace render
	{
		//////////////////////
		// VulkanMemoryPage //
		//////////////////////

		VulkanMemoryPage::VulkanMemoryPage(VulkanMemoryHeap& myHeap, Uint64 size, MemoryUsage memoryUsage, Uint32 memoryTypeIndex) :
			mMyHeap(myHeap), mMappedData(nullptr), mPage(size, 0)
		{
			VkResult res;

			VkMemoryAllocateInfo info = {};
			info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			info.pNext = nullptr;
			info.memoryTypeIndex = memoryTypeIndex;
			info.allocationSize = size;

			res = vkAllocateMemory(myHeap.mDevice->GetHandle(), &info, nullptr, &mDeviceMemory);
			CHECK_VK(res, "Failed to create VulkanMemoryPage.");

			const char* memoryUsageDebugName = "";
			switch(memoryUsage) {
				case MemoryUsage::CPU:
					memoryUsageDebugName = ", MemoryUsage: CPU";
					res = vkMapMemory(mMyHeap.mDevice->GetHandle(), mDeviceMemory, 0, size, 0, &mMappedData);
					CHECK_VK(res, "Failed to map the memory.");
					break;

				case MemoryUsage::CPUtoGPU:
					memoryUsageDebugName = ", MemoryUsage: CPUtoGPU";
					res = vkMapMemory(mMyHeap.mDevice->GetHandle(), mDeviceMemory, 0, size, 0, &mMappedData);
					CHECK_VK(res, "Failed to map the memory.");
					break;

				case MemoryUsage::GPUtoCPU:
					memoryUsageDebugName = ", MemoryUsage: GPUtoCPU";
					res = vkMapMemory(mMyHeap.mDevice->GetHandle(), mDeviceMemory, 0, size, 0, &mMappedData);
					CHECK_VK(res, "Failed to map the memory.");
					break;

				case MemoryUsage::GPU:
					memoryUsageDebugName = ", MemoryUsage: GPU";
					break;
				
				default:
					break;
			}

			VulkanDebug::SetObjectName(myHeap.mDevice->GetHandle(), mDeviceMemory,
				fmt::format("VulkanMemoryPage (Size: {0}, MemoryTypeIndex: {1} {2})",
					size, memoryTypeIndex, memoryUsageDebugName).c_str());
		}

		VulkanMemoryPage::~VulkanMemoryPage()
		{
			if(mMappedData != nullptr) {
				vkUnmapMemory(mMyHeap.mDevice->GetHandle(), mDeviceMemory);
			}

			if(mDeviceMemory != VK_NULL_HANDLE) {
				vkFreeMemory(mMyHeap.mDevice->GetHandle(), mDeviceMemory, nullptr);
			}
		}

		VulkanAllocation VulkanMemoryPage::Allocate(Uint64 size, Uint64 alignment)
		{
			VariableSizeMemoryPage::Allocation alloc = mPage.Allocate(size, alignment);
			if(alloc.offset == Uint64InvalidValue) {
				return VulkanAllocation{ VK_NULL_HANDLE, 0, 0, 0, nullptr, nullptr, false, VK_NULL_HANDLE };
			}

			void* allocMappedData = nullptr;
			if(mMappedData != nullptr) {
				allocMappedData = (char*)mMappedData + alloc.offset;
			}

			return VulkanAllocation{ mDeviceMemory, alloc.offset, alloc.unalignedOffset, size, allocMappedData, this, false, VK_NULL_HANDLE };
		}

		void VulkanMemoryPage::Free(VulkanAllocation& alloc)
		{
			VariableSizeMemoryPage::Allocation allocation{ alloc.offset, alloc.unalignedOffset, alloc.size };
			mPage.Free(allocation);
		}

		void VulkanMemoryPage::Free(VulkanAllocation&& alloc)
		{
			Free(alloc);
		}

		//////////////////////
		// VulkanMemoryHeap //
		//////////////////////

		VulkanMemoryHeap::VulkanMemoryHeap(SPtr<VulkanLogicalDevice>& device, MemoryUsage memoryUsage, Uint32 memoryTypeIndex,
			Uint64 heapSize, Uint64 pageSize) :
			mDevice(device), mMemoryUsage(memoryUsage), mMemoryTypeIndex(memoryTypeIndex),
			mHeapSize(heapSize), mPageSize(pageSize)
		{
		}

		VulkanAllocation VulkanMemoryHeap::Allocate(Uint64 size, Uint64 alignment)
		{
			VulkanAllocation allocation;

			Lock lock(mPagesMutex);

			for(auto& page : mPages) {
				allocation = page->Allocate(size, alignment);
				if(allocation.pPage != nullptr) {
					return allocation;
				}
			}

			// Create a new page
			Uint64 pageSize = mPageSize;
			while(pageSize < size + alignment)
				pageSize *= 2;

			mPages.push_back(std::make_unique<VulkanMemoryPage>(*this, pageSize, mMemoryUsage, mMemoryTypeIndex));
			allocation = mPages.back()->Allocate(size, alignment);

			return allocation;
		}

		/////////////////////////
		// VulkanMemoryManager //
		/////////////////////////

		VulkanMemoryManager::VulkanMemoryManager(SPtr<VulkanLogicalDevice>& device, Uint64 gpuPageSize, Uint64 hostVisiblePageSize) :
			mDevice(device)
		{
			auto& physicalDevice = device->GetParentPhysicalDevice();
			auto& memProperties = physicalDevice.GetMemoryProperties();
			Uint32 typeBits = (1 << memProperties.memoryTypeCount) - 1;
			bool isIntegratedGPU = (physicalDevice.GetProperties().deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU);

			VkMemoryPropertyFlags requiredFlags;
			VkMemoryPropertyFlags preferredFlags;

			// Setup GPU heap
			// TODO: UE4보니까 여러 index들을 쓰던데 왜?(VulkanMemory.cpp)
			requiredFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
			preferredFlags = 0;
			Uint32 gpuIndex = physicalDevice.GetMemoryTypeIndex(typeBits, requiredFlags, preferredFlags);
			CHECK(gpuIndex != -1, "Failed to memory type with a property that has DEVICE_LOCAL.");
			Uint64 heapSize = memProperties.memoryHeaps[memProperties.memoryTypes[gpuIndex].heapIndex].size;
			Uint64 pageSize = Math::Min(heapSize / 8, gpuPageSize);
			mHeaps[(Uint32)MemoryUsage::GPU] = VulkanMemoryHeap(device, MemoryUsage::GPU, gpuIndex, heapSize, pageSize);

			// Setup CPU heap
			requiredFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
			preferredFlags = 0;
			Uint32 cpuIndex = physicalDevice.GetMemoryTypeIndex(typeBits, requiredFlags, preferredFlags);
			CHECK(cpuIndex != -1, "Failed to memory type with a property that has HOST_VISIBLE | HOST_COHERENT.");
			heapSize = memProperties.memoryHeaps[memProperties.memoryTypes[cpuIndex].heapIndex].size;
			pageSize = hostVisiblePageSize;
			mHeaps[(Uint32)MemoryUsage::CPU] = VulkanMemoryHeap(device, MemoryUsage::CPU, cpuIndex, heapSize, pageSize);
			
			// Setup CPUtoGPU heap
			// TODO: HOST_COHERENT가 없을 경우 Flush하는 기능 추가
			bool flushManually = false;
			requiredFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
			preferredFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
			Uint32 CtoGindex = physicalDevice.GetMemoryTypeIndex(typeBits, requiredFlags, preferredFlags);
			CHECK(CtoGindex != -1,
				"Failed to memory type with a property that has HOST_VISIBLE(required), DEVICE_LOCAL(preferred).");
			heapSize = memProperties.memoryHeaps[memProperties.memoryTypes[CtoGindex].heapIndex].size;
			pageSize = hostVisiblePageSize;
			mHeaps[(Uint32)MemoryUsage::CPUtoGPU] = VulkanMemoryHeap(device, MemoryUsage::CPUtoGPU, CtoGindex, heapSize, pageSize);

			// Setup GPUtoCPU heap
			requiredFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
			preferredFlags = VK_MEMORY_PROPERTY_HOST_CACHED_BIT;
			Uint32 GtoCindex = physicalDevice.GetMemoryTypeIndex(typeBits, requiredFlags, preferredFlags);
			CHECK(GtoCindex != -1,
				"Failed to memory type with a property that has HOST_VISIBLE(required), HOST_COHERENT(prefferd).");
			heapSize = memProperties.memoryHeaps[memProperties.memoryTypes[GtoCindex].heapIndex].size;
			pageSize = hostVisiblePageSize;
			mHeaps[(Uint32)MemoryUsage::GPUtoCPU] = VulkanMemoryHeap(device, MemoryUsage::GPUtoCPU, GtoCindex, heapSize, pageSize);
		}

		VulkanMemoryManager::~VulkanMemoryManager()
		{
		}

		VulkanAllocation VulkanMemoryManager::Allocate(Uint64 size, Uint64 alignment, MemoryUsage usage)
		{
			return mHeaps[(Uint32)usage].Allocate(size, alignment);
		}
		
		VulkanAllocation VulkanMemoryManager::Allocate(const VkMemoryRequirements& memRequirements, MemoryUsage usage)
		{
#ifdef _DEBUG
			Uint32 index = mHeaps[(Uint32)usage].GetMemoryTypeIndex();
			CHECK(((1 << index) & memRequirements.memoryTypeBits) != 0,
				"Failed to allocate VulkanMemory. MemoryTypeBits in VkMemoryRequirements({0}) doesn't have index {1}",
				memRequirements.memoryTypeBits, index);
#endif // _DEBUG

			return mHeaps[(Uint32)usage].Allocate(memRequirements.size, memRequirements.alignment);
		}

		VulkanAllocation VulkanMemoryManager::AllocateDedicated(const VkMemoryRequirements2& memRequirements,
			const VkMemoryDedicatedAllocateInfo& dediAllocInfo, MemoryUsage usage)
		{
			VkResult res;

			CHECK(memRequirements.pNext != nullptr, "You must add VkMemoryDedicatedRequirements in pNext of VkMemoryRequirements2.");

			Uint32 index = mHeaps[(Uint32)usage].GetMemoryTypeIndex();
			CHECK(((1 << index) & memRequirements.memoryRequirements.memoryTypeBits) != 0,
				"Failed to allocate VulkanMemory. MemoryTypeBits in VkMemoryRequirements({0}) doesn't have index {1}",
				memRequirements.memoryRequirements.memoryTypeBits, index);

			VkMemoryAllocateInfo info = {};
			info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			info.pNext = &dediAllocInfo;
			info.memoryTypeIndex = index;
			info.allocationSize = memRequirements.memoryRequirements.size;

			VkDeviceMemory mem;
			res = vkAllocateMemory(mDevice->GetHandle(), &info, nullptr, &mem);
			CHECK_VK(res, "Failed to allocate dedicated memory.");

			// Mapped memory if its usage is not GPU
			Uint64 size = memRequirements.memoryRequirements.size;

			void* mappedData = nullptr;
			const char* memoryUsageDebugName = "";
			switch(usage) {
				case MemoryUsage::CPU:
					memoryUsageDebugName = ", MemoryUsage: CPU";
					res = vkMapMemory(mDevice->GetHandle(), mem, 0, size, 0, &mappedData);
					CHECK_VK(res, "Failed to map the memory.");
					break;

				case MemoryUsage::CPUtoGPU:
					memoryUsageDebugName = ", MemoryUsage: CPUtoGPU";
					res = vkMapMemory(mDevice->GetHandle(), mem, 0, size, 0, &mappedData);
					CHECK_VK(res, "Failed to map the memory.");
					break;

				case MemoryUsage::GPUtoCPU:
					memoryUsageDebugName = ", MemoryUsage: GPUtoCPU";
					res = vkMapMemory(mDevice->GetHandle(), mem, 0, size, 0, &mappedData);
					CHECK_VK(res, "Failed to map the memory.");
					break;

				case MemoryUsage::GPU:
					memoryUsageDebugName = ", MemoryUsage: GPU";
					break;

				default:
					break;
			}

			VulkanDebug::SetObjectName(mDevice->GetHandle(), mem,
				fmt::format("Dedicated memory allocation (Size: {0}, MemoryTypeIndex: {1} {2})",
					size, index, memoryUsageDebugName).c_str());

			VulkanAllocation alloc;
			alloc.offset = 0;
			alloc.unalignedOffset = 0;
			alloc.size = memRequirements.memoryRequirements.size;
			alloc.mappedData = mappedData;

			alloc.isDedicatedAllocation = true;
			alloc.deviceMemory = mem;
			alloc.device = mDevice->GetHandle();

			return alloc;
		}
	} // namespace render
} // namespace cube
