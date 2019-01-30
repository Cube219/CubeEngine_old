#pragma once

#include "VulkanAPIHeader.h"

#include "BaseRenderAPI_New/Utilities/VariableSizeMemoryPage.h"
#include "EngineCore/Thread/MutexLock.h"
#include "EngineCore/Assertion.h"

namespace cube
{
	namespace render
	{
		struct VulkanAllocation
		{
			VkDeviceMemory deviceMemory;
			Uint64 offset;
			Uint64 unalignedOffset;
			Uint64 size;
			void* mappedData = nullptr;

			VulkanMemoryPage* pPage = nullptr;

			bool isDedicatedAllocation;
			VkDevice device; // For freeing dedicated allocation

			void Free();
		};

		//////////////////////
		// VulkanMemoryPage //
		//////////////////////

		enum class MemoryUsage : Uint32
		{
			GPU,
			CPU,
			CPUtoGPU,
			GPUtoCPU,
			Count
		};

		class VulkanMemoryPage
		{
		public:
			VulkanMemoryPage(VulkanMemoryHeap& myHeap, Uint64 size, MemoryUsage memoryUsage, Uint32 memoryTypeIndex);
			~VulkanMemoryPage();

			VulkanAllocation Allocate(Uint64 size, Uint64 alignment);
			void Free(VulkanAllocation& alloc);
			void Free(VulkanAllocation&& alloc);

			VkDeviceMemory GetVkDeviceMemory() const { return mDeviceMemory; }

		private:
			VulkanMemoryHeap& mMyHeap;

			VariableSizeMemoryPage mPage;

			VkDeviceMemory mDeviceMemory;
			void* mMappedData;
		};

		//////////////////////
		// VulkanMemoryHeap //
		//////////////////////

		class VulkanMemoryHeap
		{
		public:
			VulkanMemoryHeap() : mDevice(nullptr)
			{}
			VulkanMemoryHeap(SPtr<VulkanLogicalDevice>& device, MemoryUsage memoryUsage, Uint32 memoryTypeIndex,
				Uint64 heapSize, Uint64 pageSize);
			~VulkanMemoryHeap() {}

			VulkanMemoryHeap(const VulkanMemoryHeap& other) = delete;
			VulkanMemoryHeap& operator=(const VulkanMemoryHeap& rhs) = delete;

			VulkanMemoryHeap(VulkanMemoryHeap&& other) : 
				mDevice(std::move(other.mDevice)),
				mMemoryTypeIndex(other.mMemoryTypeIndex),
				mHeapSize(other.mHeapSize),
				mPageSize(other.mPageSize),
				// mPagesMutex(other.mPagesMutex), // It cannot be moved.
				mPages(std::move(other.mPages))
			{}
			VulkanMemoryHeap& operator=(VulkanMemoryHeap&& rhs)
			{
				mDevice = std::move(rhs.mDevice);
				mMemoryTypeIndex = rhs.mMemoryTypeIndex;
				mHeapSize = rhs.mHeapSize;
				mPageSize = rhs.mPageSize;
				// mPagesMutex = std::move(rhs.mPagesMutex); // It cannot be moved.
				mPages = std::move(rhs.mPages);

				return *this;
			}

			VulkanAllocation Allocate(Uint64 size, Uint64 alignment);

		private:
			friend class VulkanMemoryManager;
			friend class VulkanMemoryPage;

			SPtr<VulkanLogicalDevice> mDevice;

			MemoryUsage mMemoryUsage;
			Uint32 mMemoryTypeIndex;
			Uint64 mHeapSize;
			Uint64 mPageSize;

			core::Mutex mPagesMutex;
			Vector<VulkanMemoryPage> mPages;
		};

		/////////////////////////
		// VulkanMemoryManager //
		/////////////////////////

		// TODO: Async Allocator
		class VulkanMemoryManager
		{
		public:
			VulkanMemoryManager(SPtr<VulkanLogicalDevice>& device, Uint64 gpuPageSize, Uint64 hostVisiblePageSize);
			~VulkanMemoryManager();

			VulkanMemoryManager(const VulkanMemoryManager& others) = delete;
			VulkanMemoryManager& operator=(const VulkanMemoryManager& rhs) = delete;
			VulkanMemoryManager(VulkanMemoryManager&& others) = delete;
			VulkanMemoryManager& operator=(VulkanMemoryManager&& rhs) = delete;

			VulkanAllocation Allocate(Uint64 size, Uint64 alignment, MemoryUsage usage);
			VulkanAllocation Allocate(const VkMemoryRequirements& memRequirements, MemoryUsage usage);
			VulkanAllocation AllocateDedicated(const VkMemoryRequirements2& memRequirements,
				const VkMemoryDedicatedAllocateInfo& dediAllocInfo, MemoryUsage usage);

		private:
			SPtr<VulkanLogicalDevice> mDevice;

			Array<Uint32, (Uint32)MemoryUsage::Count> mMemTypeIndexAsMemoryUsage;
			Vector<VulkanMemoryHeap> mHeaps;
		};

		inline void VulkanAllocation::Free()
		{
			if(!isDedicatedAllocation)
				pPage->Free(*this);
			else
				vkFreeMemory(device, deviceMemory, nullptr);
		}
	} // namespace render
} // namespace cube
