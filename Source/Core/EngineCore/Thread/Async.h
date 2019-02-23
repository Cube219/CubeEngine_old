#pragma once

#include "../EngineCoreHeader.h"

#include "MutexLock.h"

namespace cube
{
	class ENGINE_CORE_EXPORT AsyncSignal
	{
	public:
		AsyncSignal();

		void UpdateState(float progress);
		void DispatchCompletion();

		void Reset();

	private:
		friend class Async;

		ThreadSignal mFinishSignal;
		Mutex mMutex;
		bool mIsFinished;
		float mProgress;
	};

	class ENGINE_CORE_EXPORT Async
	{
	public:
		Async(AsyncSignal& signal) : mAsyncSignal(signal)
		{ }

		void WaitUntilFinished()
		{
			Lock lock(mAsyncSignal.mMutex);

			if(mAsyncSignal.mIsFinished == false) {
				mAsyncSignal.mFinishSignal.wait(lock);
			}
		}

		bool IsDone()
		{
			Lock lock(mAsyncSignal.mMutex);

			return mAsyncSignal.mIsFinished;
		}

		float GetProgress()
		{
			Lock lock(mAsyncSignal.mMutex);

			return mAsyncSignal.mProgress;
		}

	private:
		AsyncSignal& mAsyncSignal;
	};
} // namespace cube
