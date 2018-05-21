#include "CubeEngine.h"

#include "Platform.h"
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
	void CubeEngine::Start(const CubeEngineStartOption& startOption)
	{
		InitPlatform();

		platform::Platform::InitWindow(startOption.title, startOption.windowWidth, startOption.windowHeight);
		platform::Platform::ShowWindow();

		core::EngineCore::CreateInstance();

		core::ECore()->Prepare();

		auto rm = core::ECore()->GetRendererManager();
		core::RenderingThread::Init(rm);
		core::GameThread::Init(core::ECore());

		core::AsyncState gameThreadAsync = core::GameThread::PrepareAsync();
		core::RenderingThread::Prepare();

		gameThreadAsync.WaitUntilFinished();

		CUBE_LOG(LogType::Info, "Finished Prepareing CubeEngine");

		RegisterImporters();
		InitComponents();

		core::ECore()->SetFPSLimit(60);
	}

	void CubeEngine::Run()
	{
		core::GameThread::Run();
		core::RenderingThread::Run();
	}

	void CubeEngine::Destroy()
	{
		core::ECore()->DestroyInstance();
	}

	void CubeEngine::RegisterImporters()
	{
		SPtr<core::ResourceManager> resManager = core::ECore()->GetResourceManager();
		SPtr<render::RenderAPI> renderAPI  = core::ECore()->GetRendererManager()->GetRenderAPI();

		resManager->RegisterImporter(
			std::make_unique<TextureImporter>(renderAPI)
		);
		resManager->RegisterImporter(
			std::make_unique<ShaderImporter>(renderAPI)
		);
		resManager->RegisterImporter(
			std::make_unique<ObjImporter>()
		);
	}

	void CubeEngine::InitComponents()
	{
		SPtr<core::ComponentManager> comManager = core::ECore()->GetComponentManager();

		comManager->RegisterComponent<Renderer3DComponent>();
		comManager->RegisterComponent<CameraComponent>();
		comManager->RegisterComponent<MoveComponent>();
		comManager->RegisterComponent<DirectionalLightComponent>();
		comManager->RegisterComponent<PointLightComponent>();
	}

	////////////////////////////////
	//           WIN32            //
	////////////////////////////////

#ifdef _WIN32

} // namespace cube

#include "Win32/Win32Platform.h"

namespace cube{

	void CubeEngine::InitPlatform()
	{
		HINSTANCE ins;
		ins = GetModuleHandle(NULL);

		platform::Win32Platform::Init(ins);
	}

#endif // _WIN32

} // namespace cube
