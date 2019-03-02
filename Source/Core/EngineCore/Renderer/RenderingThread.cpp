#include "RenderingThread.h"

#include "RendererManager.h"
#include "../LogWriter.h"
#include "Platform.h"
#include "../GameThread.h"

namespace cube
{
	RendererManager* RenderingThread::mRendererManager = nullptr;

	std::thread::id RenderingThread::mThreadId;

	EventFunction<void()> RenderingThread::mLoopEventFunc;
	EventFunction<void(Uint32, Uint32)> RenderingThread::mResizeEventFunc;

	Mutex RenderingThread::mSyncTaskQueueMutex;
	TaskQueue RenderingThread::mSyncTaskQueue;

	Mutex RenderingThread::mTaskQueueMutex;
	TaskQueue RenderingThread::mTaskQueue;
	TaskQueue RenderingThread::mLastTaskQueue;

	void RenderingThread::Init(RendererManager* rendererManager)
	{
		mRendererManager = rendererManager;

		mThreadId = std::this_thread::get_id();
	}

	void RenderingThread::Prepare()
	{
		mRendererManager->Initialize(RenderType::Vulkan);

		mLoopEventFunc = platform::Platform::GetLoopEvent().AddListener(&RenderingThread::Loop);
		mResizeEventFunc = platform::Platform::GetResizeEvent().AddListener(&RenderingThread::OnResize);
	}

	void RenderingThread::PrepareDestroy()
	{
		platform::Platform::GetResizeEvent().RemoveListener(mResizeEventFunc);
		platform::Platform::GetLoopEvent().RemoveListener(mLoopEventFunc);

		mSyncTaskQueue.Flush();
		mTaskQueue.Flush();
	}

	void RenderingThread::Destroy()
	{
		mTaskQueue.ExecuteAll();
		mTaskQueue.Flush();

		mRendererManager->ShutDown();
	}

	void RenderingThread::Run(Async& gameThreadRunAsync)
	{
		gameThreadRunAsync.WaitUntilFinished();

		// For fulshing initial resources
		Sync();

		mLastTaskQueue.ExecuteAll();
		mLastTaskQueue.Flush();

		mRendererManager->DrawAll();

		platform::Platform::StartLoop();
	}

	void RenderingThread::Loop()
	{
		if(GameThread::mWillBeDestroyed == true) {
			Async async = GameThread::PrepareDestroyAsync();

			PrepareDestroy();

			async.WaitUntilFinished();

			platform::Platform::FinishLoop();

			return;
		}

		Sync();

		Async async = GameThread::ExecuteTaskQueueAndSimulateAsync();

		mLastTaskQueue.ExecuteAll();
		mLastTaskQueue.Flush();

		Rendering();

		async.WaitUntilFinished();
	}

	void RenderingThread::Sync()
	{
		mSyncTaskQueue.ExecuteAll();
		mSyncTaskQueue.Flush();

		mTaskQueue.QueueAndFlush(mLastTaskQueue);
	}

	void RenderingThread::Rendering()
	{
		mRendererManager->StartFrame();

		mRendererManager->DrawAll();
	}

	void RenderingThread::DestroyInternal()
	{
		mRendererManager->ShutDown();

		mSyncTaskQueue.Flush();
		mTaskQueue.Flush();
		mLastTaskQueue.Flush();
	}

	void RenderingThread::OnResize(Uint32 width, Uint32 height)
	{
		mRendererManager->Resize(width, height);
	}
} // namespace cube
