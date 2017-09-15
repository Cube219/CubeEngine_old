#include "EngineCore.h"

#include "InputManager.h"
#include "TimeManager.h"
#include "String\StringManager.h"
#include "LogWriter.h"
#include "Renderer\RendererManager.h"
#include "ModuleManager.h"
#include "GameObject.h"
#include "Renderer\Renderer3D.h"

#include "Renderer\Vertex.h"

namespace cube
{
	namespace core
	{
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

			Vector<Vertex> vertices;
			vertices.push_back({XYZ1(-1, -1, -1), XYZ1(1.0f, 0.0f, 0.0f)});
			vertices.push_back({XYZ1(-1, 1, -1), XYZ1(0.0f, 1.0f, 0.0f)});
			vertices.push_back({XYZ1(1, -1, -1), XYZ1(0.0f, 0.0f, 1.0f)});
			vertices.push_back({XYZ1(1, 1, -1), XYZ1(1.0f, 1.0f, 0.0f)});
			vertices.push_back({XYZ1(1, -1, 1), XYZ1(1.0f, 0.0f, 1.0f)});
			vertices.push_back({XYZ1(1, 1, 1), XYZ1(0.0f, 1.0f, 1.0f)});
			vertices.push_back({XYZ1(-1, -1, 1), XYZ1(1.0f, 1.0f, 1.0f)});
			vertices.push_back({XYZ1(-1, 1, 1), XYZ1(0.0f, 0.0f, 0.0f)});

			Vector<uint32_t> indices;
			indices.push_back(0);
			indices.push_back(1);
			indices.push_back(2);

			indices.push_back(2);
			indices.push_back(1);
			indices.push_back(3);

			indices.push_back(2);
			indices.push_back(3);
			indices.push_back(4);

			indices.push_back(4);
			indices.push_back(3);
			indices.push_back(5);

			indices.push_back(4);
			indices.push_back(5);
			indices.push_back(6);

			indices.push_back(6);
			indices.push_back(5);
			indices.push_back(7);

			indices.push_back(6);
			indices.push_back(7);
			indices.push_back(0);

			indices.push_back(0);
			indices.push_back(7);
			indices.push_back(1);

			indices.push_back(6);
			indices.push_back(0);
			indices.push_back(2);

			indices.push_back(2);
			indices.push_back(4);
			indices.push_back(6);

			indices.push_back(7);
			indices.push_back(5);
			indices.push_back(3);

			indices.push_back(7);
			indices.push_back(3);
			indices.push_back(1);

			mGo = std::make_shared<GameObject>(mRendererManager->CreateRenderer3D());

			auto renderer = mGo->GetRenderer();
			renderer->SetVertex(vertices);
			renderer->SetIndex(indices);
		}

		void EngineCore::Run()
		{
			mTimeManager->Start();

			mPlatform->StartLoop();
		}

		float EngineCore::GetCurrentFPS()
		{
			// TODO: 더 좋은 방법으로 개선
			return SCast(float)(1.0 / mTimeManager->GetDeltaTime());
		}

		void EngineCore::SetFPSLimit(int limit)
		{
			mFPSLimit = limit;
		}

		void EngineCore::Loop()
		{
			mTimeManager->Loop();

			double currentTime = mTimeManager->GetSystemTime(); // For limit FPS 

			mGo->Update();

			auto currentRotation = mGo->GetRotation();
			currentRotation.x += 5.0f;
			mGo->SetRotation(currentRotation);

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
	}
}