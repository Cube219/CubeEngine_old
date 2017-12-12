#include "EngineCore.h"

#include "InputManager.h"
#include "Time/TimeManager.h"
#include "Time/GameTime.h"
#include "String/StringManager.h"
#include "LogWriter.h"
#include "Renderer/RendererManager.h"
#include "Renderer/Mesh.h"
#include "Renderer/BaseMeshGenerator.h"
#include "Renderer/Texture.h"
#include "Renderer/Material/Material.h"
#include "Resource/ResourceManager.h"
#include "ModuleManager.h"
#include "GameObject.h"
#include "Renderer/Renderer3D.h"

namespace cube
{
	namespace core
	{
		EngineCore* EngineCore::mInstance = nullptr;

		EngineCore::EngineCore(SPtr<platform::BasePlatform>& platform) :
			mPlatform(platform), mFPSLimit(-1)
		{
			platform->SetKeyDownFunction(std::bind(&EngineCore::KeyDown, this, _1));
			platform->SetKeyUpFunction(std::bind(&EngineCore::KeyUp, this, _1));
			platform->SetMouseDownFunction(std::bind(&EngineCore::MouseDown, this, _1));
			platform->SetMouseUpFunction(std::bind(&EngineCore::MouseUp, this, _1));
			platform->SetMouseWheelFunction(std::bind(&EngineCore::MouseWheel, this, _1));
			platform->SetMousePosFunction(std::bind(&EngineCore::UpdateMousePos, this, _1, _2));

			platform->SetLoopFunction(std::bind(&EngineCore::Loop, this));
			platform->SetResizeFunction(std::bind(&EngineCore::Resize, this, _1, _2));
		}

		EngineCore::~EngineCore()
		{
		}

		void EngineCore::Prepare()
		{
			LogWriter::Init(mPlatform);

			mTimeManager = std::make_unique<TimeManager>();
			mStringManager = std::make_unique<StringManager>();

			mRendererManager = std::make_unique<RendererManager>(mPlatform, RenderType::Vulkan);

			mResourceManager = std::make_unique<ResourceManager>(mPlatform->GetFileSystem());

			mModuleManager = std::make_unique<ModuleManager>(mPlatform);

			// Create mesh / texture
			mBoxMesh = BaseMeshGenerator::GetBoxMesh();

			WString texturePath = L"Data/TestTexture.png";
			mTexture = mResourceManager->LoadResource<Texture>(texturePath);

			// Create material
			Vector<MaterialParameterInfo> paramInfos;
			paramInfos.push_back({"Texture", MaterialParameterType::Texture, 0});
			mMaterial1 = std::make_shared<Material>(paramInfos);

			String t = "Texture";
			mMaterial1->SetParameterData<SPtr<Texture>>(t, mTexture);

			// Create gameobjects
			for(int i = -5; i <= 5; i++) {
				for(int j = -5; j <= 5; j++) {
					for(int k = -5; k <= 5; k++) {
						auto go = std::make_shared<GameObject>(mRendererManager->CreateRenderer3D());
						Vector3 v;
						v.x = i;
						v.y = j;
						v.z = k;
						go->SetPosition(v);

						auto renderer = go->GetRenderer();
						renderer->SetMesh(mBoxMesh);
						renderer->SetMaterial(mMaterial1);

						mGos.push_back(go);
					}
				}
			}
		}

		void EngineCore::Run()
		{
			mTimeManager->Start();

			mPlatform->StartLoop();
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

			for(auto& go : mGos) {
				go->Update();
			}

			mRendererManager->DrawAll();

			// Limit FPS
			if(mFPSLimit > 0) {
				double nextTime = currentTime + (1.0 / mFPSLimit);

				currentTime = mTimeManager->GetSystemTime();

				double waitTime = nextTime - currentTime;

				if(waitTime > 0.1) { // TODO: 적절한 수치를 찾기
					mPlatform->Sleep(SCast(int)(waitTime * 1000));
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

		void EngineCore::KeyDown(KeyCode keyCode)
		{
			InputManager::KeyDown(keyCode);
		}

		void EngineCore::KeyUp(KeyCode keyCode)
		{
			InputManager::KeyUp(keyCode);
		}

		void EngineCore::MouseDown(MouseButtonType buttonType)
		{
			InputManager::MouseDown(buttonType);
		}

		void EngineCore::MouseUp(MouseButtonType buttonType)
		{
			InputManager::MouseUp(buttonType);
		}

		void EngineCore::MouseWheel(int wheelDelta)
		{
			InputManager::MouseWheel(wheelDelta);
		}

		void EngineCore::UpdateMousePos(uint32_t x, uint32_t y)
		{
			InputManager::UpdateMousePos(x, y);
		}

		EngineCore* ECore()
		{
			return EngineCore::GetInstance();
		}
	}
}