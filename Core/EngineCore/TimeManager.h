#pragma once

#include "EngineCoreHeader.h"

#include <chrono>

namespace cube
{
	namespace core
	{
		using TimePoint = std::chrono::time_point<std::chrono::high_resolution_clock, std::chrono::nanoseconds>;
		using Duration = std::chrono::nanoseconds;

		class TimeManager
		{
		public:
			TimeManager();
			~TimeManager();

			double GetCurrentTime();

			void Start();
			void Loop();

			void Pause();
			void Resume();

		private:
			TimePoint GetNow();

			Duration mCurrentTime;
			
			TimePoint mPreviousTimePoint;
			TimePoint mCurrentTimePoint;

			bool mIsPaused;
		};
	}
}