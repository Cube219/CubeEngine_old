#include "GameThread.h"

#include "EngineCore.h"
#include "LogWriter.h"
#include "Assertion.h"
#include "Renderer/RenderingThread.h"

namespace cube
{
	EngineCore* GameThread::mECore = nullptr;
	std::thread::id GameThread::mThreadId;
	std::thread GameThread::mMyThread;
	std::function<void()> GameThread::mRunFunction = nullptr;

	bool GameThread::mWillBeDestroyed = false;

	AsyncSignal GameThread::mStartSignal;
	AsyncSignal GameThread::mFinishSignal;
	AsyncSignal GameThread::mDestroySignal;

	Mutex GameThread::mTaskBufferMutex;
	TaskQueue GameThread::mTaskQueue;

	void GameThread::Init(EngineCore* eCore)
	{
		mECore = eCore;
		mFinishSignal.DispatchCompletion();

		mMyThread = std::thread(&GameThread::RunInternal);
		mThreadId = mMyThread.get_id();
	}

	Async GameThread::PrepareAsync()
	{
		Async a = Async(mFinishSignal);
		a.WaitUntilFinished();

		mRunFunction = &GameThread::PrepareInternal;
		mStartSignal.DispatchCompletion();

		mFinishSignal.Reset();

		return Async(mFinishSignal);
	}

	Async GameThread::SimulateAsync()
	{
		mECore->Start();

		Async a = Async(mFinishSignal);
		a.WaitUntilFinished();

		mRunFunction = &GameThread::SimulateInternal;
		mStartSignal.DispatchCompletion();

		mFinishSignal.Reset();

		return Async(mFinishSignal);
	}

	Async GameThread::ExecuteTaskQueueAndSimulateAsync()
	{
		Async a = Async(mFinishSignal);
		a.WaitUntilFinished();

		mRunFunction = &GameThread::ExecuteTaskQueueAndSimulateInternal;
		mStartSignal.DispatchCompletion();

		mFinishSignal.Reset();

		return Async(mFinishSignal);
	}

	Async GameThread::PrepareDestroyAsync()
	{
		Async a = Async(mFinishSignal);
		a.WaitUntilFinished();

		mRunFunction = &GameThread::PrepareDestroyInternal;
		mStartSignal.DispatchCompletion();

		mFinishSignal.Reset();

		return Async(mFinishSignal);
	}

	Async GameThread::DestroyAsync()
	{
		Async a = Async(mFinishSignal);
		a.WaitUntilFinished();

		mRunFunction = &GameThread::DestroyInternal;
		mStartSignal.DispatchCompletion();

		mFinishSignal.Reset();

		return Async(mFinishSignal);
	}

	void GameThread::RunInternal()
	{
		while(1) {
			{ // Wait until start
				Async a = Async(mStartSignal);
				a.WaitUntilFinished();

				mFinishSignal.Reset();
			}

			mRunFunction();

			{ // Check destroy signal
				Async a = Async(mDestroySignal);
				if(a.IsDone()) {
					break;
				}
			}

			{ // Finish
				mRunFunction = nullptr;
				mStartSignal.Reset();
				mFinishSignal.DispatchCompletion();
			}
		}

		mFinishSignal.DispatchCompletion();
	}

	void GameThread::PrepareInternal()
	{
		mECore->Initialize();
	}

	void GameThread::ExecuteTaskQueueAndSimulateInternal()
	{
		mTaskQueue.ExecuteAll();
		mTaskQueue.Flush();

		SimulateInternal();
	}

	void GameThread::SimulateInternal()
	{
		mECore->Update();
	}

	void GameThread::PrepareDestroyInternal()
	{
		mTaskQueue.Flush();
	}

	void GameThread::DestroyInternal()
	{
		mECore->ShutDown();
		mTaskQueue.Flush();

		mDestroySignal.DispatchCompletion();
	}
} // namespace cube
