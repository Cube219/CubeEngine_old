#include "VulkanUploadHeap.h"

#include "VulkanMemoryManager.h"

#include "EngineCore/Assertion.h"

namespace cube
{
	namespace render
	{
		//////////////////////
		// VulkanUploadPage //
		//////////////////////

		VulkanUploadPage::VulkanUploadPage(VulkanMemoryManager& memoryManager, Uint64 size) :
			mPageSize(size)
		{
			mAllocation = memoryManager.Allocate(size, 0, MemoryUsage::CPUtoGPU);
			mStartOffset = mAllocation.offset;
			mCurrentOffset = mStartOffset;
			mEndOffset = mCurrentOffset + mAllocation.size;
		}

		VulkanUploadPage::~VulkanUploadPage()
		{
			mAllocation.Free();
		}

		VulkanUploadAllocation VulkanUploadPage::Allocate(Uint64 size, Uint64 alignment)
		{
			Uint64 alignedOffset = Align(mCurrentOffset, alignment);
			if(alignedOffset + size > mEndOffset) {
				return VulkanUploadAllocation{ nullptr };
			}

			VulkanUploadAllocation allocation;
			allocation.mappedData = (char*)mAllocation.mappedData + alignedOffset;

			mCurrentOffset = alignedOffset + size;

			return allocation;
		}

		void VulkanUploadPage::DiscardAlloations()
		{
			mCurrentOffset = mStartOffset;
		}

		//////////////////////
		// VulkanUploadHeap //
		//////////////////////

		VulkanUploadHeap::VulkanUploadHeap(VulkanMemoryManager& memoryManager, Uint64 pageSize) : 
			mMemoryManager(memoryManager),
			mDefaultPageSize(pageSize)
		{
		}

		VulkanUploadHeap::~VulkanUploadHeap()
		{
		}

		VulkanUploadAllocation VulkanUploadHeap::Allocate(Uint64 size, Uint64 alignment)
		{
			VulkanUploadAllocation allocation;

			Lock lock(mPagesMutex);

			for(auto& page : mPages) {
				allocation = page.Allocate(size, alignment);
				if(allocation.mappedData != nullptr) {
					return allocation;
				}
			}

			// Create a new page
			Uint64 pageSize = mDefaultPageSize;
			while(pageSize < size)
				pageSize *= 2;

			mPages.emplace_back(mMemoryManager, pageSize);
			allocation = mPages.back().Allocate(size, alignment);

			return allocation;
		}

		void VulkanUploadHeap::DiscardAllocations()
		{
			for(auto& page : mPages) {
				page.DiscardAlloations();
			}

			while(mPages.size() > 1) {
				mPages.pop_back();
			}
		}
	} // namespace render
} // namespace cube
