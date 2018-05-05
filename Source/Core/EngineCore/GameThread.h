#pragma once

#include "EngineCoreHeader.h"

#include "Thread/MutexLock.h"
#include "Thread/Async.h"

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

		private:
			static void PrepareInternal();

			static EngineCore* mECore;
			static std::thread mMyThread;

			static AsyncStateData mPrepareAsyncData;
		};
	} // namespace core
} // namespace cube
