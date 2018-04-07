#include "EngineCore.h"

#include "Platform.h"
#include "Time/TimeManager.h"
#include "Time/GameTime.h"
#include "String/StringManager.h"
#include "Thread/ThreadManager.h"
#include "LogWriter.h"
#include "Renderer/RendererManager.h"
#include "Renderer/Texture.h"
#include "Renderer/Material/Shader.h"
#include "Resource/ResourceManager.h"
#include "ModuleManager.h"
#include "GameObjectManager.h"
#include "Component/ComponentManager.h"

namespace cube
{
	namespace core
	{
		EngineCore* EngineCore::mInstance = nullptr;

		EngineCore::EngineCore() :
			mFPSLimit(-1)
		{
		}

		EngineCore::~EngineCore()
		{
			platform::Platform::GetLoopEvent().RemoveListener(mLoopEventFunc);
			platform::Platform::GetResizeEvent().RemoveListener(mResizeEventFunc);
		}

		void EngineCore::Prepare()
		{
			mLoopEventFunc = platform::Platform::GetLoopEvent().AddListener(std::bind(&EngineCore::Loop, this));
			mResizeEventFunc = platform::Platform::GetResizeEvent().AddListener(std::bind(&EngineCore::Resize, this, _1, _2));

			LogWriter::Init();

			mTimeManager = std::make_unique<TimeManager>();
			mStringManager = std::make_unique<StringManager>();

			mRendererManager = std::make_unique<RendererManager>(RenderType::Vulkan);

			mResourceManager = std::make_unique<ResourceManager>(platform::Platform::GetFileSystem());
			mResourceManager->RegisterImporter(
				std::make_unique<TextureImporter>(mRendererManager->GetRenderAPI())
			);
			mResourceManager->RegisterImporter(
				std::make_unique<ShaderImporter>(mRendererManager->GetRenderAPI())
			);

			mThreadManager = std::make_shared<ThreadManager>();

			mModuleManager = std::make_shared<ModuleManager>(mThreadManager);
			mModuleManager->LoadModule(CUBE_T("InputModule"));
			mModuleManager->InitModules();

			mGameObjectManager = std::make_shared<GameObjectManager>();
			mComponentManager = std::make_shared<ComponentManager>();
		}

		void EngineCore::Run()
		{
			mTimeManager->Start();

			mGameObjectManager->Start();

			platform::Platform::StartLoop();
		}

		float EngineCore::GetCurrentFPS()
		{
			// TODO: 더 좋은 방법으로 개선
			return 1.0f / mTimeManager->GetGlobalGameTime()->GetDeltaTime();
		}

		void EngineCore::SetFPSLimit(int limit)
		{
			mFPSLimit = limit;
		}

		void EngineCore::Loop()
		{
			mTimeManager->Update();

			double currentTime = mTimeManager->GetSystemTime(); // For limit FPS 

			float dt = mTimeManager->GetGlobalGameTime()->GetDeltaTime();

			mModuleManager->UpdateAllModules(dt);

			mGameObjectManager->Update(dt);
			
			mRendererManager->DrawAll();

			// Limit FPS
			if(mFPSLimit > 0) {
				double nextTime = currentTime + (1.0 / mFPSLimit);

				currentTime = mTimeManager->GetSystemTime();

				double waitTime = nextTime - currentTime;

				if(waitTime > 0.1) { // TODO: 적절한 수치를 찾기
					platform::Platform::Sleep(SCast(int)(waitTime * 1000));
				} else if(waitTime > 0.0) {
					while(nextTime > currentTime) {
						currentTime = mTimeManager->GetSystemTime();
					}
				}
			}
		}

		void EngineCore::Resize(uint32_t width, uint32_t height)
		{
			mRendererManager->Resize(width, height);
		}

		EngineCore* ECore()
		{
			return EngineCore::GetInstance();
		}
	} // namespace core
} // namespace cube
