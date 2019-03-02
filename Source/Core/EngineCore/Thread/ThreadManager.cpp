#include "ThreadManager.h"

#include "Thread.h"

#include "../Assertion.h"

using namespace std::placeholders;

namespace cube
{
	void ThreadManager::Initialize(Uint32 poolSize)
	{
		mThreads.resize(poolSize);

		for(Uint32 i = 0; i < poolSize; i++) {
			mThreads[i] = SPtr<Thread>(new Thread(i, std::bind(&ThreadManager::OnThreadFinish, this, _1)));

			mIdleThreadIndices.push_back(i);
		}

		mIdleThreadIndices_front = 0;
		mIdleThreadIndices_back = poolSize - 1;
	}

	void ThreadManager::ShutDown()
	{
		for(auto& t : mThreads) {
			t->Destroy();
		}
	}

	SPtr<Thread> ThreadManager::GetThread()
	{
		Lock lock(mMutex);

		Uint32 nextIdleIndex = (mIdleThreadIndices_front + 1) % mThreads.size();
		CHECK(nextIdleIndex != mIdleThreadIndices_back, "There's no idle thread in the pool");

		Uint32 index = mIdleThreadIndices[mIdleThreadIndices_front];
		mIdleThreadIndices_front = nextIdleIndex;

		return mThreads[index];
	}

	void ThreadManager::OnThreadFinish(Uint32 poolIndex)
	{
		Lock lock(mMutex);

		mIdleThreadIndices_back = (mIdleThreadIndices_back + 1) % mThreads.size();
		mIdleThreadIndices[mIdleThreadIndices_back] = poolIndex;
	}
} // namespace cube
