#include "GameThread.h"

#include "EngineCore.h"
#include "LogWriter.h"
#include "Assertion.h"
#include "Renderer/RenderingThread.h"

namespace cube
{
	namespace core
	{
		EngineCore* GameThread::mECore = nullptr;
		std::thread GameThread::mMyThread;
		std::function<void()> GameThread::mRunFunction = nullptr;

		AsyncSignal GameThread::mStartSignal;
		AsyncSignal GameThread::mFinishSignal;
		AsyncSignal GameThread::mDestroySignal;

		Mutex GameThread::mTaskBufferMutex;
		TaskBuffer GameThread::mTaskBuffer;

		void GameThread::Init(EngineCore* eCore)
		{
			mECore = eCore;
			mFinishSignal.DispatchCompletion();

			mMyThread = std::thread(&GameThread::RunInternal);
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

		Async GameThread::DestroyAsync()
		{
			Async a = Async(mFinishSignal);
			a.WaitUntilFinished();

			mRunFunction = &GameThread::DestroyInternal;
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

		Async GameThread::ProcessTaskBuffersAndSimulateAsync()
		{
			Async a = Async(mFinishSignal);
			a.WaitUntilFinished();

			mRunFunction = &GameThread::ProcessTaskBuffersAndSimulateInternal;
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

		void GameThread::DestroyInternal()
		{
			mECore->ShutDown();

			mTaskBuffer.Flush();

			mDestroySignal.DispatchCompletion();
		}

		void GameThread::ProcessTaskBuffersAndSimulateInternal()
		{
			TaskBuffer& buf = RenderingThread::_GetTaskBuffer();
			buf.ExecuteAll();

			buf.Flush();

			SimulateInternal();
		}

		void GameThread::SimulateInternal()
		{
			mECore->Update();
		}
	} // namespace core
} // namespace cube
