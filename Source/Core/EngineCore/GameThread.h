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
			static void Run();
			static Async DestroyAsync();

			static Async SimulateAsync();
			static Async ProcessTaskBuffersAndSimulateAsync();

			static void Join(){ mMyThread.join(); }

			static void QueueTask(std::function<void()> taskFunc)
			{
				Lock lock(mTaskBufferMutex);

				mTaskBuffer.WriteTask(taskFunc);
			}

			static TaskBuffer& _GetTaskBuffer(){ return mTaskBuffer; }

		private:
			friend class RenderingThread;

			static void ThreadFunc();
			static void PrepareInternal();
			static void RunInternal();

			static void ProcessTaskBuffers();
			static void Simulate();

			static EngineCore* mECore;
			static std::thread mMyThread;

			static Mutex mMutex;
			static ThreadSignal mRunNotify;

			static AsyncSignal mPrepareAsyncSignal;
			static AsyncSignal mSimulateAsyncSignal;
			static AsyncSignal mProcessTaskBuffersAndSimulateAsyncSignal;
			static AsyncSignal mDestroyAsyncSignal;
			
			static AsyncSignal mSimulateNotifyAsyncSignal;
			static Async mSimulateNotifyAsync;
			static AsyncSignal mProcessTaskBuffersAndSimulateNotifyAsyncSignal;
			static Async mProcessTaskBuffersAndSimulateNotifyAsync;
			static AsyncSignal mDestroyNotifyAsyncSignal;
			static Async mDestroyNotifyAsync;

			static Mutex mTaskBufferMutex;
			static TaskBuffer mTaskBuffer;
		};
	} // namespace core
} // namespace cube
