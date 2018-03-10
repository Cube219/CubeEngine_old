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

			SPtr<RendererManager> GetRendererManager() const { return mRendererManager; }
			SPtr<TimeManager> GetTimeManager() const { return mTimeManager; }
			SPtr<StringManager> GetStringManager() const { return mStringManager; }
			SPtr<platform::BasePlatform> GetPlatform() const { return mPlatform; }

		private:
			void Loop();

			void Resize(uint32_t width, uint32_t height);

			SPtr<platform::BasePlatform> mPlatform;

			SPtr<RendererManager> mRendererManager;
			SPtr<ResourceManager> mResourceManager;
			SPtr<TimeManager> mTimeManager;
			SPtr<StringManager> mStringManager;

			SPtr<ThreadManager> mThreadManager;

			SPtr<ModuleManager> mModuleManager;

			//Vector<SPtr<GameObject>> mGameObjects;
			SPtr<Mesh> mBoxMesh;
			RPtr<Texture> mTexture;
			RPtr<Texture> mTexture2;

			//SPtr<GameObject> mGo1;
			RPtr<Shader> mMaterialVertexShader;
			RPtr<Shader> mMaterialFragmentShader;
			SPtr<Material> mMaterial;
			SPtr<MaterialInstance> mMaterialIns1;
			SPtr<MaterialInstance> mMaterialIns2;
			//SPtr<GameObject> mGo2;
			//SPtr<Material> mMaterial2;
			Vector<SPtr<GameObject>> mGos;

			int mFPSLimit;
		};

		// Helper function to get singleton instance easily
		ENGINE_CORE_EXPORT EngineCore* ECore();
	}
}
