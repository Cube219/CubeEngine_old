﻿#pragma once

#include "VulkanAPIHeader.h"

#include "EngineCore/Thread/MutexLock.h"

namespace cube
{
	namespace render
	{
		struct VulkanAllocation
		{
			VkDeviceSize offset;
			VkDeviceSize size;
			VulkanMemoryPage* pPage = nullptr;
		};

		//////////////////////
		// VulkanMemoryPage //
		//////////////////////

		class VulkanMemoryPage
		{
		public:
			VulkanMemoryPage(VulkanMemoryHeap& myHeap, VkDeviceSize size, Uint32 memoryTypeIndex);
			~VulkanMemoryPage();

			VulkanAllocation Allocate(VkDeviceSize size, VkDeviceSize alignment);
			void Free(VulkanAllocation& alloc);
			void Free(VulkanAllocation&& alloc);

			VkDeviceMemory GetVkDeviceMemory() const { return mDeviceMemory; }

		private:
			struct FreeBlock;

			using OrderByOffsetMap = Map<VkDeviceSize, FreeBlock>;
			using OrderBySizeMultiMap = MultiMap<VkDeviceSize, OrderByOffsetMap::iterator>;

			struct FreeBlock
			{
				VkDeviceSize size;
				OrderBySizeMultiMap::iterator orderBySizeIter;
				FreeBlock(VkDeviceSize size) : size(size) {}
			};

			void InsertFreeBlock(VkDeviceSize offset, VkDeviceSize size);
			void RemoveFreeBlock(OrderByOffsetMap::iterator iter);

			VulkanMemoryHeap& mMyHeap;

			VkDeviceMemory mDeviceMemory;

			VkDeviceSize mMaxSize;
			VkDeviceSize mFreeSize;
			OrderByOffsetMap mFreeBlocksOrderByOffset;
			OrderBySizeMultiMap mFreeBlocksOrderBySize;
		};

		//////////////////////
		// VulkanMemoryHeap //
		//////////////////////

		class VulkanMemoryHeap
		{
		public:
			VulkanMemoryHeap() : mDevicePtr(nullptr)
			{}
			VulkanMemoryHeap(DeviceVk* devicePtr, Uint32 memoryTypeIndex, VkDeviceSize heapSize, VkDeviceSize pageSize);
			~VulkanMemoryHeap() {}

			VulkanMemoryHeap(const VulkanMemoryHeap& other) = delete;
			VulkanMemoryHeap& operator=(const VulkanMemoryHeap& rhs) = delete;

			VulkanMemoryHeap(VulkanMemoryHeap&& other);
			VulkanMemoryHeap& operator=(VulkanMemoryHeap&& rhs);

			VulkanAllocation Allocate(VkDeviceSize size, VkDeviceSize alignment);

		private:
			friend class VulkanMemoryManager;
			friend class VulkanMemoryPage;

			DeviceVk* mDevicePtr;

			Uint32 mMemoryTypeIndex;
			VkDeviceSize mHeapSize;
			VkDeviceSize mPageSize;

			core::Mutex mPagesMutex;
			Vector<VulkanMemoryPage> mPages;
		};

		/////////////////////////
		// VulkanMemoryManager //
		/////////////////////////

		enum class MemoryUsage : Uint32
		{
			GPU,
			CPU,
			CPUtoGPU,
			GPUtoCPU,
			Count
		};

		// TODO: Async Allocator
		class VulkanMemoryManager
		{
		public:
			VulkanMemoryManager(DeviceVk& device, VkDeviceSize gpuPageSize, VkDeviceSize hostVisiblePageSize);
			~VulkanMemoryManager();

			VulkanMemoryManager(const VulkanMemoryManager& others) = delete;
			VulkanMemoryManager& operator=(const VulkanMemoryManager& rhs) = delete;
			VulkanMemoryManager(VulkanMemoryManager&& others) = delete;
			VulkanMemoryManager& operator=(VulkanMemoryManager&& rhs) = delete;

			VulkanAllocation Allocate(VkDeviceSize size, VkDeviceSize alignment, MemoryUsage usage);
			VulkanAllocation Allocate(const VkMemoryRequirements& memRequirements, MemoryUsage usage);

		private:
			VkDevice mVkDevice;

			Array<Uint32, (Uint32)MemoryUsage::Count> mHeapIndexAsMemoryUsage;
			Vector<VulkanMemoryHeap> mHeaps;
		};
	} // namespace render
} // namespace cube