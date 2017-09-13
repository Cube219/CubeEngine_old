#include "TimeManager.h"

#include "LogWriter.h"

namespace cube
{
	namespace core
	{
		TimeManager::TimeManager()
		{
		}

		TimeManager::~TimeManager()
		{
		}

		double TimeManager::GetSystemTime()
		{
			return GetNow().time_since_epoch().count() / ToSecond;
		}

		double TimeManager::GetCurrentGameTime()
		{
			return mCurrentGameTime.count() / ToSecond;
		}

		double TimeManager::GetDeltaTime()
		{
			double d = (mCurrentTimePoint - mPreviousTimePoint).count() / ToSecond;
			return d;
		}

		void TimeManager::Start()
		{
			mCurrentTimePoint = GetNow();
			mCurrentGameTime = Duration::zero();

			mIsPaused = false;
		}

		void TimeManager::Loop()
		{
			mPreviousTimePoint = mCurrentTimePoint;
			mCurrentTimePoint = GetNow();

			Duration deltaTime = mCurrentTimePoint - mPreviousTimePoint;

			if(!mIsPaused) {
				mCurrentGameTime += deltaTime;
			}
		}

		void TimeManager::Pause()
		{
			mIsPaused = true;
		}

		void TimeManager::Resume()
		{
			mIsPaused = false;
		}

		TimePoint TimeManager::GetNow()
		{
			return std::chrono::time_point_cast<Duration>(std::chrono::high_resolution_clock::now());
		}
	}
}