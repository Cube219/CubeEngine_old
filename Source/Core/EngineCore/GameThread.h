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
			static AsyncState PrepareAsync();
			static void Run();
			static AsyncState DestroyAsync();

			static AsyncState SimulateAsync();
			static AsyncState ProcessTaskBuffersAndSimulateAsync();

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
			static ThreadNotify mRunNotify;

			static AsyncStateData mPrepareAsyncData;
			static AsyncStateData mSimulateAsyncData;
			static AsyncStateData mProcessTaskBuffersAndSimulateAsyncData;
			static AsyncStateData mDestroyAsyncData;
			
			static AsyncStateData mSimulateNotifyAsyncData;
			static AsyncState mSimulateNotifyAsync;
			static AsyncStateData mProcessTaskBuffersAndSimulateNotifyAsyncData;
			static AsyncState mProcessTaskBuffersAndSimulateNotifyAsync;
			static AsyncStateData mDestroyNotifyAsyncData;
			static AsyncState mDestroyNotifyAsync;

			static Mutex mTaskBufferMutex;
			static TaskBuffer mTaskBuffer;
		};
	} // namespace core
} // namespace cube
