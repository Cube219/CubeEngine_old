#pragma once

#include "EngineCoreHeader.h"

#include "BasePlatform\BasePlatform.h"

namespace cube
{
	namespace core
	{
		class ENGINE_CORE_EXPORT EngineCore
		{
		public:
			EngineCore(SPtr<platform::BasePlatform>& platform);
			~EngineCore();

			void Prepare();

			void Run();

		private:
			void Loop();

			void Resize(uint32_t width, uint32_t height);

			void KeyDown(KeyCode keyCode);
			void KeyUp(KeyCode keyCode);
			void MouseDown(MouseButtonType buttonType);
			void MouseUp(MouseButtonType buttonType);
			void MouseWheel(int wheelDelta);
			void UpdateMousePos(uint32_t x, uint32_t y);

			SPtr<platform::BasePlatform> mPlatform;
			UPtr<RendererManager> mRendererManager;

			UPtr<ModuleManager> mModuleManager;

			//Vector<SPtr<GameObject>> mGameObjects;
			SPtr<GameObject> mGo;
		};
	}
}