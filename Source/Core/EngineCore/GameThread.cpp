#include "GameThread.h"

#include "EngineCore.h"
#include "LogWriter.h"
#include "Renderer/RenderingThread.h"
#include <chrono>

namespace cube
{
	namespace core
	{
		EngineCore* GameThread::mECore = nullptr;
		std::thread GameThread::mMyThread;

		Mutex GameThread::mMutex;
		ThreadNotify GameThread::mRunNotify;

		AsyncStateData GameThread::mPrepareAsyncData;
		AsyncStateData GameThread::mSimulateAsyncData;
		AsyncStateData GameThread::mProcessTaskBuffersAndSimulateAsyncData;
		AsyncStateData GameThread::mDestroyAsyncData;

		AsyncStateData GameThread::mSimulateNotifyAsyncData;
		AsyncState GameThread::mSimulateNotifyAsync(&GameThread::mSimulateNotifyAsyncData);
		AsyncStateData GameThread::mProcessTaskBuffersAndSimulateNotifyAsyncData;
		AsyncState GameThread::mProcessTaskBuffersAndSimulateNotifyAsync(&GameThread::mProcessTaskBuffersAndSimulateNotifyAsyncData);
		AsyncStateData GameThread::mDestroyNotifyAsyncData;
		AsyncState GameThread::mDestroyNotifyAsync(&GameThread::mDestroyNotifyAsyncData);

		Mutex GameThread::mTaskBufferMutex;
		TaskBuffer GameThread::mTaskBuffer;

		void GameThread::Init(EngineCore* eCore)
		{
			mECore = eCore;
		}

		AsyncState GameThread::PrepareAsync()
		{
			mPrepareAsyncData.Reset();

			mMyThread = std::thread(&GameThread::ThreadFunc);

			return AsyncState(&mPrepareAsyncData);
		}

		void GameThread::Run()
		{
			Lock lock(mMutex);

			mSimulateNotifyAsyncData.Reset();
			mProcessTaskBuffersAndSimulateNotifyAsyncData.Reset();

			mRunNotify.notify_all();
		}

		AsyncState GameThread::DestroyAsync()
		{
			mDestroyAsyncData.Reset();

			mDestroyNotifyAsyncData.DispatchCompletion();

			return AsyncState(&mDestroyAsyncData);
		}

		AsyncState GameThread::SimulateAsync()
		{
			mSimulateAsyncData.Reset();

			mSimulateNotifyAsyncData.DispatchCompletion();

			return AsyncState(&mSimulateAsyncData);
		}

		AsyncState GameThread::ProcessTaskBuffersAndSimulateAsync()
		{
			mProcessTaskBuffersAndSimulateAsyncData.Reset();

			mProcessTaskBuffersAndSimulateNotifyAsyncData.DispatchCompletion();

			return AsyncState(&mProcessTaskBuffersAndSimulateAsyncData);
		}

		void GameThread::ThreadFunc()
		{
			PrepareInternal();

			{
				Lock lock(mMutex);
				mRunNotify.wait(lock);
			}

			RunInternal();
		}

		void GameThread::PrepareInternal()
		{
			mECore->PrepareCore();

			mPrepareAsyncData.DispatchCompletion();		
		}

		void GameThread::RunInternal()
		{
			mSimulateNotifyAsync.WaitUntilFinished();

			mECore->Start();
			mSimulateAsyncData.DispatchCompletion();

			while(1) {
				mProcessTaskBuffersAndSimulateNotifyAsync.WaitUntilFinished();

				ProcessTaskBuffers();
				Simulate();

				if(mECore->mWillBeDestroyed == true) {
					break;
				}

				mProcessTaskBuffersAndSimulateAsyncData.DispatchCompletion();

				mProcessTaskBuffersAndSimulateNotifyAsyncData.Reset();

				/*
				{
					Lock lock(mMutex);

					mSimulateNotify.wait(lock);

					Simulate();
				}*/
			}

			AsyncState destroyRenderAsync = RenderingThread::DestroyAsync();
			
			mProcessTaskBuffersAndSimulateAsyncData.DispatchCompletion();

			destroyRenderAsync.WaitUntilFinished();

			/*mDestroyNotifyAsync.WaitUntilFinished();
			CUBE_LOG(LogType::Info, "Start destroying");
			mECore->DestroyInstance();
			CUBE_LOG(LogType::Info, "Finish destroying");
			mDestroyAsyncData.DispatchCompletion();*/
		}

		void GameThread::ProcessTaskBuffers()
		{
			TaskBuffer& buf = RenderingThread::_GetTaskBuffer();
			buf.ExecuteAll();

			buf.Flush();
		}

		void GameThread::Simulate()
		{
			mECore->Update();
		}
	} // namespace core
} // namespace cube
