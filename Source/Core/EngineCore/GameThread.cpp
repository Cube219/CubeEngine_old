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
		ThreadSignal GameThread::mRunNotify;

		AsyncSignal GameThread::mPrepareAsyncSignal;
		AsyncSignal GameThread::mSimulateAsyncSignal;
		AsyncSignal GameThread::mProcessTaskBuffersAndSimulateAsyncSignal;
		AsyncSignal GameThread::mDestroyAsyncSignal;

		AsyncSignal GameThread::mSimulateNotifyAsyncSignal;
		Async GameThread::mSimulateNotifyAsync(GameThread::mSimulateNotifyAsyncSignal);
		AsyncSignal GameThread::mProcessTaskBuffersAndSimulateNotifyAsyncSignal;
		Async GameThread::mProcessTaskBuffersAndSimulateNotifyAsync(GameThread::mProcessTaskBuffersAndSimulateNotifyAsyncSignal);
		AsyncSignal GameThread::mDestroyNotifyAsyncSignal;
		Async GameThread::mDestroyNotifyAsync(GameThread::mDestroyNotifyAsyncSignal);

		Mutex GameThread::mTaskBufferMutex;
		TaskBuffer GameThread::mTaskBuffer;

		void GameThread::Init(EngineCore* eCore)
		{
			mECore = eCore;
		}

		Async GameThread::PrepareAsync()
		{
			mPrepareAsyncSignal.Reset();

			mMyThread = std::thread(&GameThread::ThreadFunc);

			return Async(mPrepareAsyncSignal);
		}

		void GameThread::Run()
		{
			Lock lock(mMutex);

			mSimulateNotifyAsyncSignal.Reset();
			mProcessTaskBuffersAndSimulateNotifyAsyncSignal.Reset();

			mRunNotify.notify_all();
		}

		Async GameThread::DestroyAsync()
		{
			mDestroyAsyncSignal.Reset();

			mDestroyNotifyAsyncSignal.DispatchCompletion();

			return Async(mDestroyAsyncSignal);
		}

		Async GameThread::SimulateAsync()
		{
			mSimulateAsyncSignal.Reset();

			mSimulateNotifyAsyncSignal.DispatchCompletion();

			return Async(mSimulateAsyncSignal);
		}

		Async GameThread::ProcessTaskBuffersAndSimulateAsync()
		{
			mProcessTaskBuffersAndSimulateAsyncSignal.Reset();

			mProcessTaskBuffersAndSimulateNotifyAsyncSignal.DispatchCompletion();

			return Async(mProcessTaskBuffersAndSimulateAsyncSignal);
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
			mECore->InitializeCore();

			mPrepareAsyncSignal.DispatchCompletion();		
		}

		void GameThread::RunInternal()
		{
			mSimulateNotifyAsync.WaitUntilFinished();

			mECore->Start();
			mSimulateAsyncSignal.DispatchCompletion();

			while(1) {
				mProcessTaskBuffersAndSimulateNotifyAsync.WaitUntilFinished();

				ProcessTaskBuffers();
				Simulate();

				if(mECore->mWillBeDestroyed == true) {
					break;
				}

				mProcessTaskBuffersAndSimulateAsyncSignal.DispatchCompletion();

				mProcessTaskBuffersAndSimulateNotifyAsyncSignal.Reset();

				/*
				{
					Lock lock(mMutex);

					mSimulateNotify.wait(lock);

					Simulate();
				}*/
			}

			Async destroyRenderAsync = RenderingThread::DestroyAsync();
			
			mProcessTaskBuffersAndSimulateAsyncSignal.DispatchCompletion();

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
