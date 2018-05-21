#pragma once

#include "../EngineCoreHeader.h"

#include "MutexLock.h"

namespace cube
{
	namespace core
	{
		class ENGINE_CORE_EXPORT AsyncStateData
		{
		public:
			AsyncStateData();

			void UpdateState(float progress);
			void DispatchCompletion();

			void Reset();

		private:
			friend class AsyncState;

			ThreadNotify mFinishNofity;
			Mutex mMutex;
			bool mIsFinished;
			float mProgress;
		};

		class ENGINE_CORE_EXPORT AsyncState
		{
		public:
			AsyncState(AsyncStateData* dataPtr) : mAsyncDataPtr(dataPtr)
			{ }

			void WaitUntilFinished()
			{
				Lock lock(mAsyncDataPtr->mMutex);

				if(mAsyncDataPtr->mIsFinished == false) {
					mAsyncDataPtr->mFinishNofity.wait(lock);
				}
			}
			bool IsDone()
			{
				Lock lock(mAsyncDataPtr->mMutex);

				return mAsyncDataPtr->mIsFinished;
			}
			float GetProgress()
			{
				Lock lock(mAsyncDataPtr->mMutex);

				return mAsyncDataPtr->mProgress;
			}

		private:
			AsyncStateData* mAsyncDataPtr;
		};
	} // namespace core
} // namespace cube
