#include "CubeEngine.h"

#include "Platform.h"
#include "EngineCore/EngineCore.h"
#include "EngineCore/GameThread.h"
#include "EngineCore/Renderer/RenderingThread.h"
#include "EngineCore/Resource/ResourceManager.h"
#include "EngineCore/Renderer/RendererManager.h"
#include "BaseRenderAPI/RenderAPI.h"
#include "ResourceImporter/ShaderImporter.h"
#include "ResourceImporter/TextureImporter.h"
#include "ResourceImporter/ObjImporter.h"
#include "EngineCore/Component/ComponentManager.h"
#include "Component/Renderer3DComponent.h"
#include "Component/CameraComponent.h"
#include "Component/MoveComponent.h"
#include "Component/DirectionalLightComponent.h"
#include "Component/PointLightComponent.h"

namespace cube
{
	EventFunction<void()> CubeEngine::closingEventFunc;

	void CubeEngine::Initialize(const CubeEngineStartOption& startOption)
	{
		platform::Platform::Init();

		platform::Platform::InitWindow(startOption.title, startOption.windowWidth, startOption.windowHeight);
		platform::Platform::ShowWindow();

		
		core::ECore().Initialize();

		auto& rm = core::ECore().GetRendererManager();
		core::RenderingThread::Init(&rm);
		core::GameThread::Init(&core::ECore());

		core::Async gameThreadInitAsync = core::GameThread::PrepareAsync();
		core::RenderingThread::Prepare();

		gameThreadInitAsync.WaitUntilFinished();

		RegisterImporters();
		InitComponents();

		closingEventFunc = platform::Platform::GetClosingEvent().AddListener(&CubeEngine::DefaultClosingFunction);
		core::ECore().SetFPSLimit(60);
	}

	void CubeEngine::ShutDown()
	{
		platform::Platform::GetClosingEvent().RemoveListener(closingEventFunc);

		// TODO: 좀 더 좋은 구조로 만들기
		core::GameThread::Join();
		core::RenderingThread::ExecuteLastTaskBuffer();
		core::ECore().ShutDown();
	}

	void CubeEngine::Run()
	{
		core::GameThread::Run();
		core::RenderingThread::Run();
	}

	void CubeEngine::SetCustomClosingFunction(std::function<void()> func)
	{
		platform::Platform::GetClosingEvent().RemoveListener(closingEventFunc);
		closingEventFunc = platform::Platform::GetClosingEvent().AddListener(func);
	}

	void CubeEngine::RegisterImporters()
	{
		core::ResourceManager& resManager = core::ECore().GetResourceManager();
		SPtr<render::RenderAPI> renderAPI  = core::ECore().GetRendererManager().GetRenderAPI();

		resManager.RegisterImporter(
			std::make_unique<TextureImporter>(renderAPI)
		);
		resManager.RegisterImporter(
			std::make_unique<ShaderImporter>(renderAPI)
		);
		resManager.RegisterImporter(
			std::make_unique<ObjImporter>()
		);
	}

	void CubeEngine::InitComponents()
	{
		core::ComponentManager& comManager = core::ECore().GetComponentManager();

		comManager.RegisterComponent<Renderer3DComponent>();
		comManager.RegisterComponent<CameraComponent>();
		comManager.RegisterComponent<MoveComponent>();
		comManager.RegisterComponent<DirectionalLightComponent>();
		comManager.RegisterComponent<PointLightComponent>();
	}

	void CubeEngine::DefaultClosingFunction()
	{
		CUBE_LOG(LogType::Info, "Call closing function");
		core::ECore().ShutDown();
	}
} // namespace cube
