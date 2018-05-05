#include "RenderingThread.h"

#include "RendererManager.h"
#include "../LogWriter.h"

namespace cube
{
	namespace core
	{
		SPtr<RendererManager> RenderingThread::mRendererManager = nullptr;

		void RenderingThread::Init(SPtr<RendererManager>& rendererManager)
		{
			mRendererManager = rendererManager;
		}

		void RenderingThread::Prepare()
		{
			CUBE_LOG(LogType::Info, "Preparing RendererManager...");
			mRendererManager->Prepare(RenderType::Vulkan);

			CUBE_LOG(LogType::Info, "Finished Preparing RendererManager");
		}
	} // namespace core
} // namespace cube
