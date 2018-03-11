#include "CubeEngine.h"

#include "EngineCore/Component/ComponentManager.h"
#include "Component/MoveComponent.h"

namespace cube
{
	SPtr<platform::BasePlatform> CubeEngine::mPlatform = nullptr;

	void CubeEngine::Start(const CubeEngineStartOption& startOption)
	{
		mPlatform = GetPlatform();

		mPlatform->InitWindow(startOption.title, startOption.windowWidth, startOption.windowHeight);
		mPlatform->ShowWindow();

		core::EngineCore::CreateInstance(mPlatform);

		core::ECore()->Prepare();
		core::ECore()->SetFPSLimit(60);

		InitComponents();
	}

	void CubeEngine::Run()
	{
		core::ECore()->Run();
	}

	void CubeEngine::Destroy()
	{
		core::ECore()->DestroyInstance();
	}

	void CubeEngine::InitComponents()
	{
		SPtr<core::ComponentManager> comManager = core::ECore()->GetComponentManager();

		comManager->RegisterComponent<MoveComponent>();
	}

	////////////////////////////////
	//           WIN32            //
	////////////////////////////////

#ifdef _WIN32

}

#include "WinPlatform/WinPlatform.h"

namespace cube{

	SPtr<platform::BasePlatform> CubeEngine::GetPlatform()
	{
		HINSTANCE ins;
		ins = GetModuleHandle(NULL);

		return std::make_shared<platform::WinPlatform>(ins);
	}

#endif // _WIN32

}
