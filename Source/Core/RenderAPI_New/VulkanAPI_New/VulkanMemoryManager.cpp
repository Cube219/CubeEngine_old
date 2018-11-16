#include "VulkanMemoryManager.h"

#include "EngineCore/Assertion.h"
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

		VulkanMemoryPage::VulkanMemoryPage(VulkanMemoryHeap& myHeap, VkDeviceSize size, MemoryUsage memoryUsage, Uint32 memoryTypeIndex) :
			mMyHeap(myHeap), mMappedData(nullptr), mMaxSize(size), mFreeSize(size)
		{
			VkResult res;

			VkMemoryAllocateInfo info = {};
			info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			info.pNext = nullptr;
			info.memoryTypeIndex = memoryTypeIndex;
			info.allocationSize = size;

			res = vkAllocateMemory(myHeap.mDevice->GetHandle(), &info, nullptr, &mDeviceMemory);
			CHECK_VK(res, "Failed to Create VulkanMemoryPage.");

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

			InsertFreeBlock(0, size);
		}

		VulkanMemoryPage::~VulkanMemoryPage()
		{
			if(mMappedData != nullptr) {
				vkUnmapMemory(mMyHeap.mDevice->GetHandle(), mDeviceMemory);
			}

			vkFreeMemory(mMyHeap.mDevice->GetHandle(), mDeviceMemory, nullptr);
		}

		VulkanAllocation VulkanMemoryPage::Allocate(VkDeviceSize size, VkDeviceSize alignment)
		{
			CHECK((alignment & (alignment - 1)) == 0, "Alignment({0}) must be power of 2.", alignment);
			VkDeviceSize alignedSize = (size + (alignment - 1)) & ~(alignment - 1);
			
			auto smallestFreeBlockItIt = mFreeBlocksOrderBySize.lower_bound(alignedSize);
			if(smallestFreeBlockItIt == mFreeBlocksOrderBySize.end()) {
				return VulkanAllocation{ 0, 0, nullptr };
			}
			auto smallestFreeBlockIt = smallestFreeBlockItIt->second;

			VkDeviceSize oldFreeBlockOffset = smallestFreeBlockIt->first;
			VkDeviceSize oldFreeBlockSize = smallestFreeBlockIt->second.size;

			VkDeviceSize newFreeBlockOffset = oldFreeBlockOffset + alignedSize;
			VkDeviceSize newFreeBlockSize = oldFreeBlockSize - alignedSize;

			VkDeviceSize allocOffset = oldFreeBlockOffset;

			RemoveFreeBlock(smallestFreeBlockIt);
			InsertFreeBlock(newFreeBlockOffset, newFreeBlockSize);

			mFreeSize -= alignedSize;

			void* allocMappedData = nullptr;
			if(mMappedData != nullptr) {
				allocMappedData = (char*)mMappedData + allocOffset;
			}

			return VulkanAllocation{allocOffset, alignedSize, this, allocMappedData};
		}

		void VulkanMemoryPage::Free(VulkanAllocation& alloc)
		{
			VkDeviceSize freeOffset = alloc.offset;
			VkDeviceSize freeSize = alloc.size;

			auto nextBlockIter = mFreeBlocksOrderByOffset.upper_bound(freeOffset);
			VkDeviceSize nextOffset;
			VkDeviceSize nextSize;
			if(nextBlockIter != mFreeBlocksOrderByOffset.end()) {
				nextOffset = nextBlockIter->first;
				nextSize = nextBlockIter->second.size;
			} else {
				nextOffset = 0;
				nextSize = 0;
			}

			auto prevBlockIter = nextBlockIter;
			VkDeviceSize prevOffset;
			VkDeviceSize prevSize;
			if(prevBlockIter != mFreeBlocksOrderByOffset.begin()) {
				--prevBlockIter;
				prevOffset = prevBlockIter->first;
				prevSize = prevBlockIter->second.size;
			} else {
				prevBlockIter = mFreeBlocksOrderByOffset.end();
				prevOffset = 0;
				prevSize = 0;
			}

#ifdef _DEBUG
			// Check overlap between new and next
			if(nextBlockIter != mFreeBlocksOrderByOffset.end()) {
				CHECK((freeOffset + freeSize) <= nextOffset, "Invalid free block insertion. New free block overlap next free block.");
			} else {
				CHECK((freeOffset + freeSize) <= mMaxSize, "Invalid free block insertion. New free block exceeds max size.");
			}
			// Check overlap between prev and new
			if(prevBlockIter != mFreeBlocksOrderByOffset.end()) {
				CHECK((prevOffset + prevSize) <= freeOffset, "Invalid free block insertion. New free block overlap previous free block.");
			}
#endif // _DEBUG
			
			if(prevBlockIter != mFreeBlocksOrderByOffset.end() && prevOffset + prevSize == freeOffset) {
				//     |                    |                  |
				//     |<-----prevSize----->|<----freeSize---->|
				//     |                    |                  |
				// prevOffset           freeOffset

				if(nextBlockIter != mFreeBlocksOrderByOffset.end() && freeOffset + freeSize == nextOffset) {
					//     |                    |                  |                    |
					//     |<-----prevSize----->|<----freeSize---->|<-----nextSize----->|
					//     |                    |                  |                    |
					// prevOffset           freeOffset         nextOffset
					// -> Merge prev, free, next block
					RemoveFreeBlock(nextBlockIter);
					prevSize += freeSize + nextSize;
					prevBlockIter->second.size = prevSize;
				} else {
					//     |                    |                  |       |                    |
					//     |<-----prevSize----->|<----freeSize---->| ..... |<-----nextSize----->|
					//     |                    |                  |       |                    |
					// prevOffset           freeOffset                 nextOffset
					// -> Merge prev, free
					prevSize += freeSize;
					prevBlockIter->second.size = prevSize;
				}
			} else {
				//     |                    |       |                  |
				//     |<-----prevSize----->| ..... |<----freeSize---->|
				//     |                    |       |                  |
				// prevOffset                   freeOffset

				if(nextBlockIter != mFreeBlocksOrderByOffset.end() && freeOffset + freeSize == nextOffset) {
					//     |                    |       |                  |                    |
					//     |<-----prevSize----->| ..... |<----freeSize---->|<-----nextSize----->|
					//     |                    |       |                  |                    |
					// prevOffset                   freeOffset          nextOffset
					// -> Merge free, next
					RemoveFreeBlock(nextBlockIter);
					nextSize += freeSize;
					nextOffset = freeOffset;
					InsertFreeBlock(nextOffset, nextSize);
				} else {
					//     |                    |       |                  |       |                    |
					//     |<-----prevSize----->| ..... |<----freeSize---->| ..... |<-----nextSize----->|
					//     |                    |       |                  |       |                    |
					// prevOffset                   freeOffset                 nextOffset
					// -> Create new free block
					InsertFreeBlock(freeOffset, freeSize);
				}
			}

			alloc.pPage = nullptr;

			mFreeSize += freeSize;
		}

		void VulkanMemoryPage::Free(VulkanAllocation&& alloc)
		{
			Free(alloc);
		}

		void VulkanMemoryPage::InsertFreeBlock(VkDeviceSize offset, VkDeviceSize size)
		{
			auto newFreeBlockIterAndResult = mFreeBlocksOrderByOffset.emplace(offset, size);
			CHECK(newFreeBlockIterAndResult.second, "Failed to insert free block in mFreeBlocksOrderByOffset.");
			auto newFreeBlockIter = newFreeBlockIterAndResult.first;

			auto orderBySizeIter = mFreeBlocksOrderBySize.emplace(size, newFreeBlockIter);
			newFreeBlockIter->second.orderBySizeIter = orderBySizeIter;
		}

		void VulkanMemoryPage::RemoveFreeBlock(OrderByOffsetMap::iterator iter)
		{
			mFreeBlocksOrderBySize.erase(iter->second.orderBySizeIter);
			mFreeBlocksOrderByOffset.erase(iter);
		}

		//////////////////////
		// VulkanMemoryHeap //
		//////////////////////

		VulkanMemoryHeap::VulkanMemoryHeap(SPtr<VulkanLogicalDevice>& device, MemoryUsage memoryUsage, Uint32 memoryTypeIndex,
			VkDeviceSize heapSize, VkDeviceSize pageSize) :
			mDevice(device), mMemoryUsage(memoryUsage), mMemoryTypeIndex(memoryTypeIndex),
			mHeapSize(heapSize), mPageSize(pageSize)
		{
		}

		VulkanMemoryHeap::VulkanMemoryHeap(VulkanMemoryHeap&& other) :
			mDevice(std::move(other.mDevice)),
			mMemoryTypeIndex(other.mMemoryTypeIndex),
			mHeapSize(other.mHeapSize),
			mPageSize(other.mPageSize),
			// mPagesMutex(other.mPagesMutex), // It cannot be moved.
			mPages(std::move(other.mPages))
		{
		}

		VulkanMemoryHeap& VulkanMemoryHeap::operator=(VulkanMemoryHeap&& rhs)
		{
			mDevice = std::move(rhs.mDevice);
			mMemoryTypeIndex = rhs.mMemoryTypeIndex;
			mHeapSize = rhs.mHeapSize;
			mPageSize = rhs.mPageSize;
			// mPagesMutex = std::move(rhs.mPagesMutex); // It cannot be moved.
			mPages = std::move(rhs.mPages);

			return *this;
		}

		VulkanAllocation VulkanMemoryHeap::Allocate(VkDeviceSize size, VkDeviceSize alignment)
		{
			VulkanAllocation allocation;

			core::Lock lock(mPagesMutex);

			for(auto& page : mPages) {
				allocation = page.Allocate(size, alignment);
				if(allocation.pPage != nullptr) {
					return allocation;
				}
			}

			// Create a new page
			VkDeviceSize pageSize = mPageSize;
			while(pageSize < size)
				pageSize *= 2;

			mPages.emplace_back(*this, pageSize, mMemoryUsage, mMemoryTypeIndex);
			allocation = mPages.back().Allocate(size, alignment);

			return allocation;
		}

		/////////////////////////
		// VulkanMemoryManager //
		/////////////////////////

		VulkanMemoryManager::VulkanMemoryManager(SPtr<VulkanLogicalDevice>& device, VkDeviceSize gpuPageSize, VkDeviceSize hostVisiblePageSize) :
			mDevice(device)
		{
			auto& physicalDevice = device->GetParentPhysicalDevice();
			auto memProperties = physicalDevice.GetMemoryProperties();
			mHeaps.resize(memProperties.memoryTypeCount);
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
			VkDeviceSize heapSize = memProperties.memoryHeaps[memProperties.memoryTypes[gpuIndex].heapIndex].size;
			VkDeviceSize pageSize = Math::Min(heapSize / 8, gpuPageSize);
			mHeaps[gpuIndex] = VulkanMemoryHeap(device, MemoryUsage::GPU, gpuIndex, heapSize, pageSize);
			mHeapIndexAsMemoryUsage[(Uint32)MemoryUsage::GPU] = gpuIndex;

			// Setup CPU heap
			requiredFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
			preferredFlags = 0;
			Uint32 cpuIndex = physicalDevice.GetMemoryTypeIndex(typeBits, requiredFlags, preferredFlags);
			CHECK(cpuIndex != -1, "Failed to memory type with a property that has HOST_VISIBLE | HOST_COHERENT.");
			heapSize = memProperties.memoryHeaps[memProperties.memoryTypes[cpuIndex].heapIndex].size;
			pageSize = hostVisiblePageSize;
			if(mHeaps[cpuIndex].mDevice == nullptr)
				mHeaps[cpuIndex] = VulkanMemoryHeap(device, MemoryUsage::CPU, cpuIndex, heapSize, pageSize);
			mHeapIndexAsMemoryUsage[(Uint32)MemoryUsage::CPU] = cpuIndex;
			
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
			if(mHeaps[CtoGindex].mDevice == nullptr)
				mHeaps[CtoGindex] = VulkanMemoryHeap(device, MemoryUsage::CPUtoGPU, CtoGindex, heapSize, pageSize);
			mHeapIndexAsMemoryUsage[(Uint32)MemoryUsage::CPUtoGPU] = CtoGindex;

			// Setup GPUtoCPU heap
			requiredFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
			preferredFlags = VK_MEMORY_PROPERTY_HOST_CACHED_BIT;
			Uint32 GtoCindex = physicalDevice.GetMemoryTypeIndex(typeBits, requiredFlags, preferredFlags);
			CHECK(GtoCindex != -1,
				"Failed to memory type with a property that has HOST_VISIBLE(required), HOST_COHERENT(prefferd).");
			heapSize = memProperties.memoryHeaps[memProperties.memoryTypes[GtoCindex].heapIndex].size;
			pageSize = hostVisiblePageSize;
			if(mHeaps[GtoCindex].mDevice == nullptr)
				mHeaps[GtoCindex] = VulkanMemoryHeap(device, MemoryUsage::GPUtoCPU, GtoCindex, heapSize, pageSize);
			mHeapIndexAsMemoryUsage[(Uint32)MemoryUsage::GPUtoCPU] = GtoCindex;
		}

		VulkanMemoryManager::~VulkanMemoryManager()
		{
		}

		VulkanAllocation VulkanMemoryManager::Allocate(VkDeviceSize size, VkDeviceSize alignment, MemoryUsage usage)
		{
			Uint32 index = mHeapIndexAsMemoryUsage[(Uint32)usage];
			return mHeaps[index].Allocate(size, alignment);
		}
		
		VulkanAllocation VulkanMemoryManager::Allocate(const VkMemoryRequirements& memRequirements, MemoryUsage usage)
		{
			Uint32 index = mHeapIndexAsMemoryUsage[(Uint32)usage];
			CHECK((index & memRequirements.memoryTypeBits) != 0,
				"Failed to allocate VulkanMemory. MemoryTypeBits in VkMemoryRequirements({0}) doesn't have index {1}",
				memRequirements.memoryTypeBits, index);

			return mHeaps[index].Allocate(memRequirements.size, memRequirements.alignment);
		}
	} // namespace render
} // namespace cube
