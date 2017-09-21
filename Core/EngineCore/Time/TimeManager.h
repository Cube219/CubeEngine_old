#pragma once

#include "../EngineCoreHeader.h"

#include <chrono>

namespace cube
{
	namespace core
	{
		using TimePoint = std::chrono::time_point<std::chrono::high_resolution_clock, std::chrono::nanoseconds>;
		using Duration = std::chrono::nanoseconds;

		constexpr double systemTimeRatio = 1000000000.0;

		class TimeManager
		{
		public:
			TimeManager();
			~TimeManager();

			double GetSystemTime();

			SPtr<GameTime> GetGlobalGameTime() const { return mGlobalGameTime; };

			SPtr<GameTime> CreateGameTime();

			void Start();

			void Update();

		private:
			uint64_t GetNow() const;

			uint64_t mPreviousSystemTimePoint;
			uint64_t mCurrentSystemTimePoint;

			SPtr<GameTime> mGlobalGameTime;
			Vector<SPtr<GameTime>> mGameTimes;
		};
	}
}