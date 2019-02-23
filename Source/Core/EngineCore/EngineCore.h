#pragma once

#include "EngineCoreHeader.h"

#include "Base/Event.h"
#include "Thread/MutexLock.h"

#include "Time/TimeManager.h"
#include "String/StringManager.h"
#include "Thread/ThreadManager.h"
#include "Renderer/RendererManager.h"
#include "Resource/ResourceManager.h"
#include "Module/ModuleManager.h"
#include "GameObjectManager.h"
#include "Component/ComponentManager.h"

namespace cube
{
	class ENGINE_CORE_EXPORT EngineCore
	{
	public:
		EngineCore() : mFPSLimit(-1)
		{}
		~EngineCore() {}

		EngineCore(const EngineCore& other) = delete;
		EngineCore& operator=(const EngineCore& rhs) = delete;
		EngineCore(EngineCore&& other) = delete;
		EngineCore& operator=(EngineCore&& rhs) = delete;

		void PreInitialize();
		void Initialize();
		void ShutDown();

		void Start();

		float GetCurrentFPS();

		void SetFPSLimit(int limit);

		RendererManager& GetRendererManager() { return mRendererManager; }
		ResourceManager& GetResourceManager() { return mResourceManager; }
		TimeManager& GetTimeManager() { return mTimeManager; }
		StringManager& GetStringManager() { return mStringManager; }
		ModuleManager& GetModuleManager() { return mModuleManager; }
		ComponentManager& GetComponentManager() { return mComponentManager; }
		GameObjectManager& GetGameObjectManager() { return mGameObjectManager; }

	private:
		friend class CubeEngine;
		friend class RendererManager;
		friend class GameThread;

		void Update();

		void Resize(Uint32 width, Uint32 height);

		RendererManager mRendererManager;

		ResourceManager mResourceManager;
		TimeManager mTimeManager;
		StringManager mStringManager;

		ThreadManager mThreadManager;

		ModuleManager mModuleManager;
		ComponentManager mComponentManager;
		GameObjectManager mGameObjectManager;

		int mFPSLimit;
		bool mWillBeDestroyed = false; // Used in GameThread
	};

	ENGINE_CORE_EXPORT EngineCore& ECore();
} // namespace cube
