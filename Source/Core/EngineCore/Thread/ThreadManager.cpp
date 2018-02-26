#include "ThreadManager.h"

#include "Thread.h"

#include "../LogWriter.h"

namespace cube
{
	namespace core
	{
		ThreadManager::ThreadManager(uint32_t poolSize)
		{
			mThreads.resize(poolSize);

			for(uint32_t i = 0; i < poolSize; i++) {
				mThreads[i] = SPtr<Thread>(new Thread(i, std::bind(&ThreadManager::OnThreadFinish, this, _1)));

				mIdleThreadIndices.push_back(i);
			}

			mIdleThreadIndices_front = 0;
			mIdleThreadIndices_back = poolSize - 1;
		}

		ThreadManager::~ThreadManager()
		{
			for(auto& t : mThreads) {
				t->Destroy();
			}
		}

		SPtr<Thread> ThreadManager::GetThread()
		{
			Lock lock(mMutex);

			uint32_t nextIdleIndex = (mIdleThreadIndices_front + 1) % mThreads.size();
			if(nextIdleIndex == mIdleThreadIndices_back) {
				CUBE_LOG(LogType::Error, L"There's no idle thread in the pool.");
				return nullptr;
			}

			uint32_t index = mIdleThreadIndices[mIdleThreadIndices_front];
			mIdleThreadIndices_front = nextIdleIndex;

			return mThreads[index];
		}

		void ThreadManager::OnThreadFinish(uint32_t poolIndex)
		{
			Lock lock(mMutex);

			mIdleThreadIndices_back = (mIdleThreadIndices_back + 1) % mThreads.size();
			mIdleThreadIndices[mIdleThreadIndices_back] = poolIndex;
		}
	}
}