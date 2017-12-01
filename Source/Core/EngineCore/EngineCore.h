#pragma once

#include "EngineCoreHeader.h"

#include <iostream>

#include "BasePlatform/BasePlatform.h"

namespace cube
{
	namespace core
	{
		class ENGINE_CORE_EXPORT EngineCore
		{
		// Singleton definition
		public:
			static void CreateInstance(SPtr<platform::BasePlatform>& platform)
			{
				if(mInstance == nullptr)
					mInstance = new EngineCore(platform);
				else
					std::wcout << L"EngineCore: Instance is already created" << std::endl;
			}
			static void DestroyInstance()
			{
				if(mInstance != nullptr)
					delete mInstance;
				else
					std::wcout << L"EngineCore: Instance is not created" << std::endl;
			}
			static EngineCore* GetInstance()
			{
				return mInstance;
			}

		private:
			EngineCore(SPtr<platform::BasePlatform>& platform);
			~EngineCore();

			static EngineCore* mInstance;

		// Actual declaration
		public:
			void Prepare();

			void Run();

			float GetCurrentFPS();

			void SetFPSLimit(int limit);

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
			UPtr<TimeManager> mTimeManager;
			UPtr<StringManager> mStringManager;

			UPtr<ModuleManager> mModuleManager;

			//Vector<SPtr<GameObject>> mGameObjects;
			SPtr<Mesh> mBoxMesh;
			SPtr<Texture> mTexture;

			SPtr<GameObject> mGo1;
			SPtr<Material> mMaterial1;
			SPtr<GameObject> mGo2;
			SPtr<Material> mMaterial2;

			int mFPSLimit;
		};

		// Helper function to get singleton instance easily
		EngineCore* ECore()
		{
			return EngineCore::GetInstance();
		}
	}
}