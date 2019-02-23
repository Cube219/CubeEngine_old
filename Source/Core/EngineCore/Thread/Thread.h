#pragma once

#include "../EngineCoreHeader.h"

#include "MutexLock.h"

#include <thread>

namespace cube
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
		Thread(Uint32 poolIndex, std::function<void(Uint32)> onFinishCallback);

		void Run();

		Mutex mMutex;

		ThreadSignal mStartNotify;
		ThreadSignal mIdleNotify;

		std::function<void(Uint32)> mOnFinishCallback;

		std::thread mThread;
		std::function<void()> mRunFunction;

		Uint32 mPoolIndex; // Use in ThreadManger

		bool mIsRun;
		bool mIsDestroyed;
	};
} // namespace cube
