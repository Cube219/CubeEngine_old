#include "EngineCore.h"

#include "Platform.h"
#include "Time/GameTime.h"
#include "LogWriter.h"
#include "Renderer/Texture.h"
#include "Renderer/Material/Shader.h"

namespace cube
{
	static EngineCore gEngineCore;

	void EngineCore::PreInitialize()
	{
		LogWriter::Init();
	}

	void EngineCore::Initialize()
	{
		// mRendererManager will be initialized in RenderingThread

		mTimeManager.Initialize();
		mStringManager.Initialize();

		mResourceManager.Initialize();

		mModuleManager.Initialize();
		mModuleManager.LoadModule(CUBE_T("InputModule"));
		mModuleManager.InitModules();

		mGameObjectManager.Initialize();
		mComponentManager.Initialize();
		
	}

	void EngineCore::ShutDown()
	{
		// mRendererManager will be shutdown in RenderingThread

		mComponentManager.ShutDown();
		mGameObjectManager.ShutDown();

		mModuleManager.ShutDown();

		mResourceManager.ShutDown();

		mStringManager.ShutDown();
		mTimeManager.ShutDown();

		mWillBeDestroyed = true;
	}

	void EngineCore::Start()
	{
		mTimeManager.Start();

		mGameObjectManager.Start();
	}

	float EngineCore::GetCurrentFPS()
	{
		// TODO: 더 좋은 방법으로 개선
		return 1.0f / mTimeManager.GetGlobalGameTime()->GetDeltaTime();
	}

	void EngineCore::SetFPSLimit(int limit)
	{
		mFPSLimit = limit;
	}

	void EngineCore::Update()
	{
		mTimeManager.Update();

		double currentTime = mTimeManager.GetSystemTime(); // For limit FPS 

		float dt = mTimeManager.GetGlobalGameTime()->GetDeltaTime();

		mModuleManager.UpdateAllModules(dt);

		mGameObjectManager.Update(dt);

		// Limit FPS
		if(mFPSLimit > 0) {
			double nextTime = currentTime + (1.0 / mFPSLimit);

			currentTime = mTimeManager.GetSystemTime();

			double waitTime = nextTime - currentTime;

			if(waitTime > 0.1) { // TODO: 적절한 수치를 찾기
				platform::Platform::Sleep(SCast(int)(waitTime * 1000));
			} else if(waitTime > 0.0) {
				while(nextTime > currentTime) {
					currentTime = mTimeManager.GetSystemTime();
				}
			}
		}
	}

	void EngineCore::Resize(Uint32 width, Uint32 height)
	{
		mRendererManager.Resize(width, height);
	}

	EngineCore& ECore()
	{
		return gEngineCore;
	}
} // namespace cube
