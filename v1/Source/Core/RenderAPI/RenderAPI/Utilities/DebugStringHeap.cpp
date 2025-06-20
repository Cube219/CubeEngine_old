#include "DebugStringHeap.h"
#include <memory.h>

namespace cube
{
	namespace render
	{
		/////////////////////
		// DebugStringPage //
		/////////////////////

		DebugStringPage::DebugStringPage(Uint64 size) : 
			mSize(size)
		{
			mStartPtr = (char*)malloc(size);
			mEndPtr = mStartPtr + size;

			mCurrentPtr = mStartPtr;
		}

		DebugStringPage::~DebugStringPage()
		{
			free(mStartPtr);
		}

		const char* DebugStringPage::Allocate(const char* str)
		{
			const char* res = (const char*)mCurrentPtr;

			while(*str != '\0') {
				if(mCurrentPtr + 1 >= mEndPtr) {
					return nullptr;
				}

				*mCurrentPtr = *str;
				
				mCurrentPtr++;
				str++;
			}

			// Copy a null charactor '\0'
			*mCurrentPtr = *str;
			mCurrentPtr++;

			return res;
		}

		/////////////////////
		// DebugStringHeap //
		/////////////////////

		bool DebugStringHeap::mIsEnabled;

		Uint64 DebugStringHeap::mPageSize;

		Mutex DebugStringHeap::mPagesMutex;
		Vector<DebugStringPage> DebugStringHeap::mPages;

		void DebugStringHeap::Enable(Uint64 pageSize)
		{
			mPageSize = pageSize;
			mPages.emplace_back(pageSize);

			mIsEnabled = true;
		}

		void DebugStringHeap::Disable()
		{
			mPages.clear();

			mIsEnabled = false;
		}

		const char* DebugStringHeap::Allocate(const char* str)
		{
			if(mIsEnabled == false)
				return "";

			Lock lock(mPagesMutex);

			for(auto& page : mPages) {
				const char* res = page.Allocate(str);
				
				if(res != nullptr) {
					return res;
				}
			}

			// Create a new page
			mPages.emplace_back(mPageSize);

			return mPages.back().Allocate(str);
		}
	} // namespace render
} // namespace cube
