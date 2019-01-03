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

		AsyncSignal RenderingThread::mDestroyAsyncSignal;

		AsyncSignal RenderingThread::mDestroyNotifyAsyncSignal;
		Async RenderingThread::mDestroyNotifyAsync(RenderingThread::mDestroyNotifyAsyncSignal);

		Mutex RenderingThread::mTaskBufferMutex;
		TaskBuffer RenderingThread::mTaskBuffer;

		void RenderingThread::Init(RendererManager* rendererManager)
		{
			mRendererManager = rendererManager;
		}

		void RenderingThread::Prepare()
		{
			mRendererManager->Prepare(RenderType::Vulkan);

			mLoopEventFunc = platform::Platform::GetLoopEvent().AddListener(&RenderingThread::Loop);
			mResizeEventFunc = platform::Platform::GetResizeEvent().AddListener(&RenderingThread::OnResize);
		}

		void RenderingThread::Run()
		{
			Async simulateAsync = GameThread::SimulateAsync();
			simulateAsync.WaitUntilFinished();

			platform::Platform::StartLoop();
		}

		Async RenderingThread::DestroyAsync()
		{
			mDestroyAsyncSignal.Reset();

			mDestroyNotifyAsyncSignal.DispatchCompletion();

			return Async(mDestroyAsyncSignal);
		}

		void RenderingThread::ExecuteLastTaskBuffer()
		{
			ProcessTaskBuffers();
		}

		void RenderingThread::Loop()
		{
			ProcessTaskBuffers();

			if(mDestroyNotifyAsync.IsDone() == true) {
				mRendererManager = nullptr; // It is actually destroyed in EngineCore's destructor
				// TODO: 따로 제거되게?

				platform::Platform::FinishLoop();

				mDestroyAsyncSignal.DispatchCompletion();
				return;
			}

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

		void RenderingThread::OnResize(uint32_t width, uint32_t height)
		{
			mRendererManager->Resize(width, height);
		}
	} // namespace core
} // namespace cube
