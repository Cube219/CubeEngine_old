#include "VulkanMemoryManager.h"

#include "EngineCore/Assertion.h"
#include "VulkanUtility.h"
#include "VulkanDebug.h"
#include "Interface/DeviceVk.h"
#include "VulkanPhysicalDevice.h"
#include "Base/Math.h"

namespace cube
{
	namespace render
	{
		//////////////////////
		// VulkanMemoryPage //
		//////////////////////

		VulkanMemoryPage::VulkanMemoryPage(VulkanMemoryHeap& myHeap, VkDeviceSize size, Uint32 memoryTypeIndex) :
			mMyHeap(myHeap), mMaxSize(size), mFreeSize(size)
		{
			VkResult res;

			VkMemoryAllocateInfo info = {};
			info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			info.pNext = nullptr;
			info.memoryTypeIndex = memoryTypeIndex;
			info.allocationSize = size;

			res = vkAllocateMemory(myHeap.mDevice->GetHandle(), &info, nullptr, &mDeviceMemory);
			CheckVkResult("Failed to Create VulkanMemoryPage.", res);

			VulkanDebug::SetObjectName(myHeap.mDevice->GetHandle(), mDeviceMemory,
				fmt::format("VulkanMemoryPage (Size: {0}, MemoryTypeIndex: {1})", size, memoryTypeIndex).c_str());

			InsertFreeBlock(0, size);
		}

		VulkanMemoryPage::~VulkanMemoryPage()
		{
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

			return VulkanAllocation{allocOffset, alignedSize, this};
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

		VulkanMemoryHeap::VulkanMemoryHeap(const SPtr<DeviceVk>& device, Uint32 memoryTypeIndex, VkDeviceSize heapSize, VkDeviceSize pageSize) :
			mDevice(device), mMemoryTypeIndex(memoryTypeIndex), mHeapSize(heapSize), mPageSize(pageSize)
		{
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

			mPages.emplace_back(*this, pageSize, mMemoryTypeIndex);
			allocation = mPages.back().Allocate(size, alignment);

			return allocation;
		}

		/////////////////////////
		// VulkanMemoryManager //
		/////////////////////////

		VulkanMemoryManager::VulkanMemoryManager(SPtr<DeviceVk> device, VkDeviceSize gpuPageSize, VkDeviceSize hostVisiblePageSize) :
			mDevice(device)
		{
			auto physicalDevice = device->GetParentPhysicalDevice();
			auto memProperties = physicalDevice->GetMemoryProperties();
			mHeaps.resize(memProperties.memoryTypeCount);
			Uint32 typeBits = (1 << memProperties.memoryTypeCount) - 1;

			// Setup GPU heap
			// TODO: UE4보니까 여러 index들을 쓰던데 왜?(VulkanMemory.cpp)
			Uint32 gpuIndex = physicalDevice->GetMemoryTypeIndex(typeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
			CHECK(gpuIndex != -1, "Failed to memory type with a property that has VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT.");
			VkDeviceSize heapSize = memProperties.memoryHeaps[memProperties.memoryTypes[gpuIndex].heapIndex].size;
			VkDeviceSize pageSize = Math::Min(heapSize / 8, gpuPageSize);
			mHeaps[gpuIndex] = std::make_unique<VulkanMemoryHeap>(device, gpuIndex, heapSize, pageSize);
			mHeapIndexAsMemoryUsage[(Uint32)MemoryUsage::GPU] = gpuIndex;

			// Setup CPUtoGPU heap
			Uint32 CtoGindex = physicalDevice->GetMemoryTypeIndex(typeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
			CHECK(gpuIndex != -1, "Failed to memory type with a property that has VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT.");
			heapSize = memProperties.memoryHeaps[memProperties.memoryTypes[CtoGindex].heapIndex].size;
			pageSize = hostVisiblePageSize;
			if(gpuIndex != CtoGindex)
				mHeaps[CtoGindex] = std::make_unique<VulkanMemoryHeap>(device, CtoGindex, heapSize, pageSize);
			mHeapIndexAsMemoryUsage[(Uint32)MemoryUsage::CPUtoGPU] = CtoGindex;

			// Setup GPUtoCPU heap
			Uint32 hostVisibleCachedIndex = physicalDevice->GetMemoryTypeIndex(typeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_CACHED_BIT);
			Uint32 hostVisibleIndex = physicalDevice->GetMemoryTypeIndex(typeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
			Uint32 GtoCindex;
			if(hostVisibleCachedIndex != -1) {
				GtoCindex = hostVisibleCachedIndex;
			} else if(hostVisibleIndex != -1) {
				GtoCindex = hostVisibleIndex;
			} else {
				ASSERTION_FAILED("Failed to memory type with a property that has VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT.");
			}
			heapSize = memProperties.memoryHeaps[memProperties.memoryTypes[GtoCindex].heapIndex].size;
			pageSize = hostVisibleCachedIndex;
			if(gpuIndex != GtoCindex)
				mHeaps[GtoCindex] = std::make_unique<VulkanMemoryHeap>(device, GtoCindex, heapSize, pageSize);
			mHeapIndexAsMemoryUsage[(Uint32)MemoryUsage::GPUtoCPU] = GtoCindex;
		}

		VulkanMemoryManager::~VulkanMemoryManager()
		{
		}

		VulkanAllocation VulkanMemoryManager::Allocate(VkDeviceSize size, VkDeviceSize alignment, MemoryUsage usage)
		{
			Uint32 index = mHeapIndexAsMemoryUsage[(Uint32)usage];
			return mHeaps[index]->Allocate(size, alignment);
		}
		
		VulkanAllocation VulkanMemoryManager::Allocate(const VkMemoryRequirements& memRequirements, MemoryUsage usage)
		{
			Uint32 index = mHeapIndexAsMemoryUsage[(Uint32)usage];
			CHECK((index & memRequirements.memoryTypeBits) != 0,
				"Failed to allocate VulkanMemory. MemoryTypeBits in VkMemoryRequirements({0}) doesn't have index {1}",
				memRequirements.memoryTypeBits, index);

			return mHeaps[index]->Allocate(memRequirements.size, memRequirements.alignment);
		}
	} // namespace render
} // namespace cube
