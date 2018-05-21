#pragma once

#include "../EngineCoreHeader.h"

#include "Base/Event.h"
#include "../Thread/MutexLock.h"
#include "../Thread/TaskBuffer.h"

namespace cube
{
	namespace core
	{
		class ENGINE_CORE_EXPORT RenderingThread
		{
		public:
			RenderingThread() = delete;
			~RenderingThread() = delete;

			static void Init(SPtr<RendererManager>& rendererManager);
			static void Prepare();
			static void Run();

			static void QueueTask(std::function<void()> taskFunc)
			{
				Lock lock(mTaskBufferMutex);

				mTaskBuffer.WriteTask(taskFunc);
			}

			static TaskBuffer& _GetTaskBuffer() { return mTaskBuffer; }

		private:
			friend class GameThread;

			static void Loop();
			static void ProcessTaskBuffers();
			static void Rendering();

			static SPtr<RendererManager> mRendererManager;

			static EventFunction<void()> mLoopEventFunc;

			static Mutex mTaskBufferMutex;
			static TaskBuffer mTaskBuffer;
		};
	} // namespace core
} // namespace cube
