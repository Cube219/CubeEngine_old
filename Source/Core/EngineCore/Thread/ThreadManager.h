#pragma once

#include "../EngineCoreHeader.h"

#include "MutexLock.h"

namespace cube
{
	class ENGINE_CORE_EXPORT ThreadManager
	{
	public:
		ThreadManager() {}
		~ThreadManager() {}

		ThreadManager(const ThreadManager& other) = delete;
		ThreadManager& operator=(const ThreadManager& rhs) = delete;
		ThreadManager(ThreadManager&& other) = delete;
		ThreadManager& operator=(ThreadManager&& rhs) = delete;

		void Initialize(Uint32 poolSize = 8);
		void ShutDown();

		SPtr<Thread> GetThread();

	private:
		void OnThreadFinish(Uint32 poolIndex);

		Mutex mMutex;

		Vector<SPtr<Thread>> mThreads;

		// Treated like circular queue
		Vector<Uint32> mIdleThreadIndices;
		Uint32 mIdleThreadIndices_front;
		Uint32 mIdleThreadIndices_back;
	};
} // namespace cube
