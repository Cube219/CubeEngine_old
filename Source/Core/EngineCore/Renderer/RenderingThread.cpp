#include "RenderingThread.h"

#include "RendererManager.h"
#include "../LogWriter.h"
#include "Platform.h"
#include "../GameThread.h"

namespace cube
{
	namespace core
	{
		RendererManager* RenderingThread::mRendererManager = nullptr;

		EventFunction<void()> RenderingThread::mLoopEventFunc;
		EventFunction<void(uint32_t, uint32_t)> RenderingThread::mResizeEventFunc;

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

		void RenderingThread::Destroy()
		{
			platform::Platform::GetResizeEvent().RemoveListener(mResizeEventFunc);
			platform::Platform::GetLoopEvent().RemoveListener(mLoopEventFunc);

			mRendererManager->ShutDown();
		}

		void RenderingThread::Run(Async& gameThreadRunAsync)
		{
			gameThreadRunAsync.WaitUntilFinished();

			platform::Platform::StartLoop();
		}

		void RenderingThread::ExecuteLastTaskBuffer()
		{
			ProcessTaskBuffers();
		}

		void RenderingThread::Loop()
		{
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
			mRendererManager->DrawAll();
		}

		void RenderingThread::DestroyInternal()
		{
			mRendererManager->ShutDown();

			mTaskBuffer.Flush();
		}

		void RenderingThread::OnResize(uint32_t width, uint32_t height)
		{
			mRendererManager->Resize(width, height);
		}
	} // namespace core
} // namespace cube
