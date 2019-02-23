#pragma once

#include "VulkanAPIHeader.h"

#include "VulkanMemoryManager.h"
#include "VkObject.h"

#include "EngineCore/Thread/MutexLock.h"

namespace cube
{
	namespace render
	{
		struct VulkanUploadAllocation
		{
			void* mappedData;
		};

		class VulkanUploadPage
		{
		public:
			VulkanUploadPage(VulkanMemoryManager& memoryManager, Uint64 size);
			~VulkanUploadPage();

			VulkanUploadAllocation Allocate(Uint64 size, Uint64 alignment);

			void DiscardAlloations();

		private:
			VulkanAllocation mAllocation;

			Uint64 mPageSize;
			Uint64 mStartOffset;
			Uint64 mCurrentOffset;
			Uint64 mEndOffset;
		};

		class VulkanUploadHeap
		{
		public:
			VulkanUploadHeap(VulkanMemoryManager& memoryManager, Uint64 pageSize);
			~VulkanUploadHeap();

			VulkanUploadAllocation Allocate(Uint64 size, Uint64 alignment);

			void DiscardAllocations();

		private:
			VulkanMemoryManager& mMemoryManager;

			Mutex mPagesMutex;
			Vector<VulkanUploadPage> mPages;
			Uint64 mDefaultPageSize;
		};
	} // namespace render
} // namespace cube
