#pragma once

#include "../EngineCoreHeader.h"

#include <chrono>

namespace cube
{
	namespace core
	{
		using TimePoint = std::chrono::time_point<std::chrono::high_resolution_clock, std::chrono::nanoseconds>;
		using Duration = std::chrono::nanoseconds;

		constexpr double ToSecond = 1000000000.0;

		class TimeManager
		{
		public:
			TimeManager();
			~TimeManager();

			double GetSystemTime();

			double GetCurrentGameTime();
			double GetDeltaTime();

			void Start();
			void Loop();

			void Pause();
			void Resume();

		private:
			TimePoint GetNow();

			Duration mCurrentGameTime;
			
			TimePoint mPreviousTimePoint;
			TimePoint mCurrentTimePoint;

			bool mIsPaused;
		};
	}
}