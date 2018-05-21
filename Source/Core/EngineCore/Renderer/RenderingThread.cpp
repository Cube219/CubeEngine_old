#include "RenderingThread.h"

#include "RendererManager.h"
#include "../LogWriter.h"
#include "Platform.h"
#include "../GameThread.h"

namespace cube
{
	namespace core
	{
		SPtr<RendererManager> RenderingThread::mRendererManager = nullptr;

		EventFunction<void()> RenderingThread::mLoopEventFunc;

		Mutex RenderingThread::mTaskBufferMutex;
		TaskBuffer RenderingThread::mTaskBuffer;

		void RenderingThread::Init(SPtr<RendererManager>& rendererManager)
		{
			mRendererManager = rendererManager;
		}

		void RenderingThread::Prepare()
		{
			mRendererManager->Prepare(RenderType::Vulkan);
		}

		void RenderingThread::Run()
		{
			mLoopEventFunc = platform::Platform::GetLoopEvent().AddListener(&RenderingThread::Loop);

			AsyncState simulateAsync = GameThread::SimulateAsync();
			simulateAsync.WaitUntilFinished();

			platform::Platform::StartLoop();
		}

		void RenderingThread::Loop()
		{
			ProcessTaskBuffers();

			AsyncState async = GameThread::ProcessTaskBuffersAndSimulateAsync();

			Rendering();

			async.WaitUntilFinished();
		}

		void RenderingThread::ProcessTaskBuffers()
		{
			//CUBE_LOG(LogType::Info, "Start processing task buffer in GameThread...");
			
			TaskBuffer& buf = GameThread::_GetTaskBuffer();
			buf.ExecuteAll();

			buf.Flush();

			//CUBE_LOG(LogType::Info, "Finished processing task buffer in GameThread...");
		}

		void RenderingThread::Rendering()
		{
			//CUBE_LOG(LogType::Info, "Start drawing...");

			mRendererManager->DrawAll();

			//CUBE_LOG(LogType::Info, "Finished drawing...");
		}
	} // namespace core
} // namespace cube
