#pragma once

#include "../EngineCoreHeader.h"

#include "MutexLock.h"

namespace cube
{
	namespace core
	{
		class ENGINE_CORE_EXPORT ThreadManager
		{
		public:
			ThreadManager(uint32_t poolSize = 8);
			~ThreadManager();

			SPtr<Thread> GetThread();

		private:
			void OnThreadFinish(uint32_t poolIndex);

			Mutex mMutex;

			Vector<SPtr<Thread>> mThreads;

			// Treated like circular queue
			Vector<uint32_t> mIdleThreadIndices;
			uint32_t mIdleThreadIndices_front;
			uint32_t mIdleThreadIndices_back;
		};
	}
}