#pragma once

#include "../EngineCoreHeader.h"

#include "MutexLock.h"

#include <thread>

namespace cube
{
	namespace core
	{
		class ENGINE_CORE_EXPORT Thread
		{
		public:
			friend class ThreadManager;

			~Thread();

			void Start(std::function<void()> function);

			void WaitUntilFinished();

			void Destroy();

			bool IsRun();

		private:
			Thread(uint32_t poolIndex, std::function<void(uint32_t)> onFinishCallback);

			void Run();

			Mutex mMutex;

			ThreadSignal mStartNotify;
			ThreadSignal mIdleNotify;

			std::function<void(uint32_t)> mOnFinishCallback;

			std::thread mThread;
			std::function<void()> mRunFunction;

			uint32_t mPoolIndex; // Use in ThreadManger

			bool mIsRun;
			bool mIsDestroyed;
		};
	}
}