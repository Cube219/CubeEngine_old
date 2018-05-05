#include "GameThread.h"

#include "EngineCore.h"
#include "LogWriter.h"
#include <chrono>

namespace cube
{
	namespace core
	{
		EngineCore* GameThread::mECore = nullptr;
		std::thread GameThread::mMyThread;

		AsyncStateData GameThread::mPrepareAsyncData;

		void GameThread::Init(EngineCore* eCore)
		{
			mECore = eCore;
		}

		AsyncState GameThread::PrepareAsync()
		{
			mPrepareAsyncData.Reset();

			mMyThread = std::thread(&GameThread::PrepareInternal);

			return AsyncState(mPrepareAsyncData);
		}

		void GameThread::PrepareInternal()
		{
			CUBE_LOG(LogType::Info, "Preparing EngineCore...");
			mECore->PrepareCore();

			//using namespace std::chrono_literals;
			//std::this_thread::sleep_for(8s);

			mPrepareAsyncData.DispatchCompletion();
			CUBE_LOG(LogType::Info, "Finished Preparing EngineCore");
		}
	} // namespace core
} // namespace cube
