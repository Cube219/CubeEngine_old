#include "RenderingThread.h"

#include "RendererManager.h"
#include "../LogWriter.h"
#include "Platform.h"
#include "../GameThread.h"

namespace cube
{
	RendererManager* RenderingThread::mRendererManager = nullptr;

	EventFunction<void()> RenderingThread::mLoopEventFunc;
	EventFunction<void(Uint32, Uint32)> RenderingThread::mResizeEventFunc;

	Mutex RenderingThread::mTaskBufferMutex;
	TaskBuffer RenderingThread::mTaskBuffer;

	void RenderingThread::Init(RendererManager* rendererManager)
	{
		mRendererManager = rendererManager;
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

		mTaskBuffer.Flush();
	}

	void RenderingThread::Destroy()
	{
		mRendererManager->ShutDown();
	}

	void RenderingThread::Run(Async& gameThreadRunAsync)
	{
		gameThreadRunAsync.WaitUntilFinished();

		// For fulshing initial resources
		mRendererManager->DrawAll();

		platform::Platform::StartLoop();
	}

	void RenderingThread::ExecuteLastTaskBuffer()
	{
		ProcessTaskBuffers();
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

		ProcessTaskBuffers();

		Async async = GameThread::ProcessTaskBuffersAndSimulateAsync();

		Rendering();

		async.WaitUntilFinished();
	}

	void RenderingThread::ProcessTaskBuffers()
	{
		TaskBuffer& buf = GameThread::_GetTaskBuffer();
		buf.ExecuteAll();

		buf.Flush();
	}

	void RenderingThread::Rendering()
	{
		mRendererManager->StartFrame();

		mRendererManager->DrawAll();
	}

	void RenderingThread::DestroyInternal()
	{
		mRendererManager->ShutDown();

		mTaskBuffer.Flush();
	}

	void RenderingThread::OnResize(Uint32 width, Uint32 height)
	{
		mRendererManager->Resize(width, height);
	}
} // namespace cube
