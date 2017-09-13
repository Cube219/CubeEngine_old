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

		double TimeManager::GetCurrentTime()
		{
			return mCurrentTime.count() / 1000000000.0;
		}

		void TimeManager::Start()
		{
			mCurrentTimePoint = GetNow();
			mCurrentTime = Duration::zero();

			mIsPaused = false;
		}

		void TimeManager::Loop()
		{
			mPreviousTimePoint = mCurrentTimePoint;
			mCurrentTimePoint = GetNow();

			Duration deltaTime = mCurrentTimePoint - mPreviousTimePoint;

			if(!mIsPaused) {
				mCurrentTime += deltaTime;
			}

			LogWriter::WriteLog(std::to_wstring(GetCurrentTime()));
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