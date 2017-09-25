#pragma once

#include "../EngineCoreHeader.h"

namespace cube
{
	namespace core
	{
		constexpr float timeRatio = 1000000000.0f;

		class ENGINE_CORE_EXPORT GameTime
		{
		public:
			friend class TimeManager;

			GameTime();
			~GameTime();

			float GetCurrentTime() const { return mCurrentTime / timeRatio; };
			float GetDeltaTime() const { return (mCurrentTimePoint - mPreviousTimePoint) / timeRatio; };

			bool IsPaused() const { return mIsPaused; };


			void SetTimeScale(float scale) { mTimeScale = scale; };

			void Start();
			void Restart();

			void Pause();
			void Resume();

		private:
			void Update(uint64_t systemDeltaTime);

			bool mIsStarted;
			bool mIsPaused;

			uint64_t mCurrentTime;
			float mTimeScale;

			uint64_t mPreviousTimePoint;
			uint64_t mCurrentTimePoint;
		};
	}
}