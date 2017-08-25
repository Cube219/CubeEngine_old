#include "EngineCore.h"

#include "InputManager.h"
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
			mPlatform(platform)
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
			mPlatform->StartLoop();
		}

		void EngineCore::Loop()
		{
			mGo->Update();

			mRendererManager->DrawAll();
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