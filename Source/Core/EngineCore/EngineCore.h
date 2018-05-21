#pragma once

#include "EngineCoreHeader.h"

#include <iostream>

#include "Base/Event.h"
#include "Thread/MutexLock.h"

namespace cube
{
	namespace core
	{
		class ENGINE_CORE_EXPORT EngineCore
		{
		// Singleton definition
		public:
			static void CreateInstance()
			{
				if(mInstance == nullptr)
					mInstance = new EngineCore();
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
			EngineCore();
			~EngineCore();

			static EngineCore* mInstance;

		// Actual declaration
		public:
			void Prepare();

			void Start();

			float GetCurrentFPS();

			void SetFPSLimit(int limit);

			SPtr<RendererManager> GetRendererManager() const { return mRendererManager; }
			SPtr<ResourceManager> GetResourceManager() const { return mResourceManager; }
			SPtr<TimeManager> GetTimeManager() const { return mTimeManager; }
			SPtr<StringManager> GetStringManager() const { return mStringManager; }
			SPtr<ModuleManager> GetModuleManager() const { return mModuleManager; }
			SPtr<ComponentManager> GetComponentManager() const { return mComponentManager; }
			SPtr<GameObjectManager> GetGameObjectManager() const { return mGameObjectManager; }

		private:
			friend class CubeEngine;
			friend class RendererManager;
			friend class GameThread;

			void PrepareCore();

			void Update();
			EventFunction<void()> mLoopEventFunc;

			void Resize(uint32_t width, uint32_t height);
			EventFunction<void(uint32_t, uint32_t)> mResizeEventFunc;

			SPtr<RendererManager> mRendererManager;

			SPtr<ResourceManager> mResourceManager;
			SPtr<TimeManager> mTimeManager;
			SPtr<StringManager> mStringManager;

			SPtr<ThreadManager> mThreadManager;

			SPtr<ModuleManager> mModuleManager;
			SPtr<ComponentManager> mComponentManager;
			SPtr<GameObjectManager> mGameObjectManager;

			int mFPSLimit;
		};

		// Helper function to get singleton instance easily
		ENGINE_CORE_EXPORT EngineCore* ECore();
	} // namespace core
} // namespace cube
