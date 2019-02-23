#include "Async.h"

namespace cube
{
	AsyncSignal::AsyncSignal() : 
		mProgress(0.0f), mIsFinished(false)
	{
	}

	void AsyncSignal::UpdateState(float progress)
	{
		Lock lock(mMutex);

		mProgress = progress;
	}

	void AsyncSignal::DispatchCompletion()
	{
		Lock lock(mMutex);

		mProgress = 1.0f;
		mIsFinished = true;
		mFinishSignal.notify_all();
	}

	void AsyncSignal::Reset()
	{
		Lock lock(mMutex);

		mProgress = 0.0f;
		mIsFinished = false;
	}
} // namesapce cube
