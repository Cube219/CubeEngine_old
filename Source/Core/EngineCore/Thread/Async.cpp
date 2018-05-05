#include "Async.h"

namespace cube
{
	namespace core
	{
		AsyncStateData::AsyncStateData() : 
			mProgress(0.0f), mIsFinished(false)
		{
		}

		void AsyncStateData::UpdateState(float progress)
		{
			Lock lock(mMutex);

			mProgress = progress;
		}

		void AsyncStateData::DispatchCompletion()
		{
			Lock lock(mMutex);

			mProgress = 1.0f;
			mIsFinished = true;
			mFinishNofity.notify_all();
		}

		void AsyncStateData::Reset()
		{
			Lock lock(mMutex);

			mProgress = 0.0f;
			mIsFinished = false;
		}
	} // namespace core
} // namesapce cube
