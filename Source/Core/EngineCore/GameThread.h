#pragma once

#include "EngineCoreHeader.h"

#include "Thread/MutexLock.h"
#include "Thread/Async.h"
#include "Thread/TaskBuffer.h"

namespace cube
{
	namespace core
	{
		class ENGINE_CORE_EXPORT GameThread
		{
		public:
			GameThread(EngineCore* eCore) = delete;
			~GameThread() = delete;

			static void Init(EngineCore* eCore);

			static Async PrepareAsync();

			static Async SimulateAsync();
			static Async ProcessTaskBuffersAndSimulateAsync();

			static Async PrepareDestroyAsync();
			static Async DestroyAsync();

			static void SetDestroy() { mWillBeDestroyed = true; }

			static void Join(){ mMyThread.join(); }

			static void QueueTask(std::function<void()> taskFunc)
			{
				Lock lock(mTaskBufferMutex);

				mTaskBuffer.WriteTask(taskFunc);
			}

			static TaskBuffer& _GetTaskBuffer(){ return mTaskBuffer; }

		private:
			friend class RenderingThread;

			static void RunInternal();

			static void PrepareInternal();

			static void ProcessTaskBuffersAndSimulateInternal();
			static void SimulateInternal();

			static void PrepareDestroyInternal();
			static void DestroyInternal();

			static EngineCore* mECore;
			static std::thread mMyThread;
			static std::function<void()> mRunFunction;

			static bool mWillBeDestroyed;

			static AsyncSignal mStartSignal;
			static AsyncSignal mFinishSignal;
			static AsyncSignal mDestroySignal;

			static Mutex mTaskBufferMutex;
			static TaskBuffer mTaskBuffer;
		};
	} // namespace core
} // namespace cube
