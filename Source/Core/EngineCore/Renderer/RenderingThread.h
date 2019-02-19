#pragma once

#include "../EngineCoreHeader.h"

#include "Base/Event.h"
#include "../Thread/MutexLock.h"
#include "../Thread/TaskBuffer.h"
#include "../Thread/Async.h"

namespace cube
{
	namespace core
	{
		class ENGINE_CORE_EXPORT RenderingThread
		{
		public:
			RenderingThread() = delete;
			~RenderingThread() = delete;

			static void Init(RendererManager* rendererManager);

			static void Prepare();
			static void PrepareDestroy();
			static void Destroy();

			static void Run(Async& gameThreadRunAsync);
			static void ExecuteLastTaskBuffer();

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
			static void DestroyInternal();

			static void OnResize(uint32_t width, uint32_t height);

			static RendererManager* mRendererManager;

			static EventFunction<void()> mLoopEventFunc;
			static EventFunction<void(uint32_t, uint32_t)> mResizeEventFunc;

			static Mutex mTaskBufferMutex;
			static TaskBuffer mTaskBuffer;
		};
	} // namespace core
} // namespace cube
