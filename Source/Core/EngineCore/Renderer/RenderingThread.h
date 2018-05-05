#pragma once

#include "../EngineCoreHeader.h"

#include "../Thread/MutexLock.h"

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

		private:
			static SPtr<RendererManager> mRendererManager;
		};
	} // namespace core
} // namespace cube
