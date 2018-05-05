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
			AsyncState(AsyncStateData& data) : mAsyncData(data)
			{ }

			void WaitUntilFinished()
			{
				Lock lock(mAsyncData.mMutex);

				if(mAsyncData.mIsFinished == false) {
					mAsyncData.mFinishNofity.wait(lock);
				}
			}
			bool IsDone()
			{
				Lock lock(mAsyncData.mMutex);

				return mAsyncData.mIsFinished;
			}
			float GetProgress()
			{
				Lock lock(mAsyncData.mMutex);

				return mAsyncData.mProgress;
			}

		private:
			AsyncStateData& mAsyncData;
		};
	} // namespace core
} // namespace cube
