#pragma once

#include "../RenderAPIHeader.h"
#include "EngineCore/Thread/MutexLock.h"

namespace cube
{
	namespace render
	{
		/////////////////////
		// DebugStringPage //
		/////////////////////

		class DebugStringPage
		{
		public:
			DebugStringPage(Uint64 size);
			~DebugStringPage();

			const char* Allocate(const char* str);

		private:
			char* mStartPtr;
			char* mCurrentPtr;
			char* mEndPtr;
			Uint64 mSize;
		};

		/////////////////////
		// DebugStringHeap //
		/////////////////////

		class DebugStringHeap
		{
		public:
			DebugStringHeap() = delete;
			~DebugStringHeap() = delete;

			static void Enable(Uint64 pageSize = 1*1024*1024); // 1MiB
			static void Disable();

			static const char* Allocate(const char* str);

		private:
			static bool mIsEnabled;

			static Uint64 mPageSize;
			
			static Mutex mPagesMutex;
			static Vector<DebugStringPage> mPages;
		};
	} // namespace render
} // namespace cube
