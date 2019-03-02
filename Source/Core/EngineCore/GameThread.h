#pragma once

#include "EngineCoreHeader.h"

#include "Thread/MutexLock.h"
#include "Thread/Async.h"
#include "Thread/TaskQueue.h"

namespace cube
{
	class ENGINE_CORE_EXPORT GameThread
	{
	public:
		GameThread(EngineCore* eCore) = delete;
		~GameThread() = delete;

		static void Init(EngineCore* eCore);

		static Async PrepareAsync();

		static Async SimulateAsync();
		static Async ExecuteTaskQueueAndSimulateAsync();

		static Async PrepareDestroyAsync();
		static Async DestroyAsync();

		static void SetDestroy() { mWillBeDestroyed = true; }

		static void Join(){ mMyThread.join(); }

		static void QueueTask(std::function<void()> taskFunc)
		{
			Lock lock(mTaskBufferMutex);

			mTaskQueue.QueueTask(taskFunc);
		}

		static std::thread::id GetThreadId() { return mThreadId; }

	private:
		friend class RenderingThread;

		static void RunInternal();

		static void PrepareInternal();

		static void ExecuteTaskQueueAndSimulateInternal();
		static void SimulateInternal();

		static void PrepareDestroyInternal();
		static void DestroyInternal();

		static EngineCore* mECore;
		static std::thread mMyThread;
		static std::thread::id mThreadId;
		static std::function<void()> mRunFunction;

		static bool mWillBeDestroyed;

		static AsyncSignal mStartSignal;
		static AsyncSignal mFinishSignal;
		static AsyncSignal mDestroySignal;

		static Mutex mTaskBufferMutex;
		static TaskQueue mTaskQueue;
	};
} // namespace cube

#ifdef _DEBUG

#include "Assertion.h"
#define CHECK_IF_GAME_THREAD CHECK(cube::GameThread::GetThreadId() == std::this_thread::get_id(), "This function should be called in game thread.");
#define CHECK_IF_NOT_GAME_THREAD CHECK(cube::GameThread::GetThreadId() != std::this_thread::get_id(), "This function should not be called in game thread.");

#else

#define CHECK_IF_GAME_THREAD
#define CHECK_IF_NOT_GAME_THREAD

#endif // _DEBUG
