#pragma once

#include "../EngineCoreHeader.h"

#include <chrono>

namespace cube
{
	using TimePoint = std::chrono::time_point<std::chrono::high_resolution_clock, std::chrono::nanoseconds>;
	using Duration = std::chrono::nanoseconds;

	constexpr double systemTimeRatio = 1000000000.0;

	class ENGINE_CORE_EXPORT TimeManager
	{
	public:
		TimeManager() {}
		~TimeManager() {}

		TimeManager(const TimeManager& other) = delete;
		TimeManager& operator=(const TimeManager& rhs) = delete;
		TimeManager(TimeManager&& other) = delete;
		TimeManager& operator=(TimeManager&& rhs) = delete;

		void Initialize();
		void ShutDown();

		double GetSystemTime();

		SPtr<GameTime> GetGlobalGameTime() const { return mGlobalGameTime; };

		SPtr<GameTime> CreateGameTime();

		void Start();

		void Update();

	private:
		Uint64 GetNow() const;

		Uint64 mPreviousSystemTimePoint;
		Uint64 mCurrentSystemTimePoint;

		SPtr<GameTime> mGlobalGameTime;
		Vector<SPtr<GameTime>> mGameTimes;
	};
} // namespace cube
