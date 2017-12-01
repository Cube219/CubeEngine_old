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
#include "ModuleManager.h"
#include "GameObject.h"
#include "Renderer/Renderer3D.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

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

			mModuleManager = std::make_unique<ModuleManager>(mPlatform);

			// Create mesh / texture
			mBoxMesh = BaseMeshGenerator::GetBoxMesh();

			int width, height, channel;
			stbi_uc* p = stbi_load("Data/TestTexture.png", &width, &height, &channel, STBI_rgb_alpha);
			mTexture = std::make_shared<Texture>(mRendererManager, (char*)p, width * height * 4, width, height);
			stbi_image_free(p);

			// Create gameobjects
			mGo1 = std::make_shared<GameObject>(mRendererManager->CreateRenderer3D());
			mGo1->SetPosition({2.0f, 0.0f, 0.0f});

			auto renderer = mGo1->GetRenderer();

			renderer->SetMesh(mBoxMesh);

			Vector<MaterialParameterInfo> paramInfos;
			paramInfos.push_back({"Texture", MaterialParameterType::Texture, 0});
			mMaterial1 = std::make_shared<Material>(paramInfos);

			String t = "Texture";
			mMaterial1->SetParameterData<SPtr<Texture>>(t, mTexture);

			renderer->SetMaterial(mMaterial1);
			// 2
			mGo2 = std::make_shared<GameObject>(mRendererManager->CreateRenderer3D());
			mGo2->SetPosition({-2.0f, 0.0f, 0.0f});

			renderer = mGo2->GetRenderer();

			renderer->SetMesh(mBoxMesh);

			paramInfos.clear();
			paramInfos.push_back({"Texture", MaterialParameterType::Texture, 0});
			mMaterial2 = std::make_shared<Material>(paramInfos);

			mMaterial2->SetParameterData<SPtr<Texture>>(t, mTexture);

			renderer->SetMaterial(mMaterial2);
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

			mGo1->Update();
			mGo2->Update();

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