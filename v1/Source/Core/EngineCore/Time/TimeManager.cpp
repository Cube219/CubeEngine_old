#include "TimeManager.h"

#include "../LogWriter.h"

#include "GameTime.h"

namespace cube
{

	void TimeManager::Initialize()
	{
		mGlobalGameTime = std::make_shared<GameTime>();
	}

	void TimeManager::ShutDown()
	{
	}

	double TimeManager::GetSystemTime()
	{
		return GetNow() / systemTimeRatio;
	}

	SPtr<GameTime> TimeManager::CreateGameTime()
	{
		auto g = std::make_shared<GameTime>();
		mGameTimes.push_back(g);

		return g;
	}

	void TimeManager::Start()
	{
		mCurrentSystemTimePoint = GetNow();

		mGlobalGameTime->Start();
	}

	void TimeManager::Update()
	{
		mPreviousSystemTimePoint = mCurrentSystemTimePoint;
		mCurrentSystemTimePoint = GetNow();

		Uint64 systemDeltaTime = mCurrentSystemTimePoint - mPreviousSystemTimePoint;

		mGlobalGameTime->Update(systemDeltaTime);
		for(auto& time : mGameTimes) {
			time->Update(systemDeltaTime);
		}
	}

	Uint64 TimeManager::GetNow() const
	{
		return std::chrono::time_point_cast<Duration>(std::chrono::high_resolution_clock::now()).time_since_epoch().count();
	}
} // namespace cube
