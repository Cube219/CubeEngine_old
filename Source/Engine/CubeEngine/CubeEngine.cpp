#include "CubeEngine.h"

#include "Platform.h"
#include "EngineCore/EngineCore.h"
#include "EngineCore/GameThread.h"
#include "EngineCore/Renderer/RenderingThread.h"
#include "EngineCore/Resource/ResourceManager.h"
#include "EngineCore/Renderer/RendererManager.h"
#include "ResourceImporter/ShaderImporter.h"
#include "ResourceImporter/TextureImporter.h"
#include "ResourceImporter/ObjImporter.h"
#include "EngineCore/Component/ComponentManager.h"
#include "Component/Renderer3DComponent.h"
#include "Component/CameraComponent.h"
#include "Component/MoveComponent.h"
#include "Component/DirectionalLightComponent.h"
#include "Component/PointLightComponent.h"
#include "RenderAPI/Interface/Device.h"

namespace cube
{
	EventFunction<void()> CubeEngine::closingEventFunc;

	void CubeEngine::Initialize(const CubeEngineStartOption& startOption)
	{
		platform::Platform::Init();

		platform::Platform::InitWindow(startOption.title, startOption.windowWidth, startOption.windowHeight);
		platform::Platform::ShowWindow();

		
		core::ECore().PreInitialize();

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

		core::Async async = core::GameThread::DestroyAsync();
		async.WaitUntilFinished();

		core::RenderingThread::Destroy();

		core::GameThread::Join();
	}

	void CubeEngine::Run()
	{
		core::Async async = core::GameThread::SimulateAsync();
		core::RenderingThread::Run(async);
	}

	void CubeEngine::Close()
	{
		platform::Platform::FinishLoop();
	}

	void CubeEngine::SetCustomClosingFunction(std::function<void()> func)
	{
		platform::Platform::GetClosingEvent().RemoveListener(closingEventFunc);
		closingEventFunc = platform::Platform::GetClosingEvent().AddListener(func);
	}

	void CubeEngine::RegisterImporters()
	{
		core::ResourceManager& resManager = core::ECore().GetResourceManager();
		SPtr<render::Device> device = core::ECore().GetRendererManager().GetDevice();

		resManager.RegisterImporter(
			std::make_unique<TextureImporter>(device)
		);
		resManager.RegisterImporter(
			std::make_unique<ShaderImporter>(device)
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
		Close();
	}
} // namespace cube
