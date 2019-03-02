#pragma once

#include "../EngineCoreHeader.h"

#include "Base/Event.h"
#include "../Thread/MutexLock.h"
#include "../Thread/TaskQueue.h"
#include "../Thread/Async.h"

namespace cube
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

		static void QueueSyncTask(std::function<void()> taskFunc)
		{
			Lock lock(mSyncTaskQueueMutex);

			mSyncTaskQueue.QueueTask(taskFunc);
		}

		static void QueueTask(std::function<void()> taskFunc)
		{
			Lock lock(mTaskQueueMutex);

			mTaskQueue.QueueTask(taskFunc);
		}

		static std::thread::id GetThreadId() { return mThreadId; }

	private:
		friend class GameThread;

		static void Loop();
		static void Sync();
		static void Rendering();
		static void DestroyInternal();

		static void OnResize(Uint32 width, Uint32 height);

		static RendererManager* mRendererManager;

		static std::thread::id mThreadId;

		static EventFunction<void()> mLoopEventFunc;
		static EventFunction<void(Uint32, Uint32)> mResizeEventFunc;

		static Mutex mSyncTaskQueueMutex;
		static TaskQueue mSyncTaskQueue;

		static Mutex mTaskQueueMutex;
		static TaskQueue mTaskQueue;
		static TaskQueue mLastTaskQueue;
	};
} // namespace cube

#ifdef _DEBUG

#include "../Assertion.h"
#define CHECK_IF_RENDERING_THREAD CHECK(cube::RenderingThread::GetThreadId() == std::this_thread::get_id(), "This function should be called in rendering thread.");
#define CHECK_IF_NOT_RENDERING_THREAD CHECK(cube::RenderingThread::GetThreadId() != std::this_thread::get_id(), "This function should not be called in rendering thread.");

#else

#define CHECK_IF_RENDERING_THREAD
#define CHECK_IF_NOT_RENDERING_THREAD

#endif // _DEBUG
