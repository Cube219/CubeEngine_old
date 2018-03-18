#include "CubeEngine.h"

#include "Platform.h"
#include "EngineCore/Component/ComponentManager.h"
#include "Component/CameraComponent.h"
#include "Component/MoveComponent.h"
#include "EngineCore/GameObject.h"

namespace cube
{
	void CubeEngine::Start(const CubeEngineStartOption& startOption)
	{
		InitPlatform();

		platform::Platform::InitWindow(startOption.title, startOption.windowWidth, startOption.windowHeight);
		platform::Platform::ShowWindow();

		core::EngineCore::CreateInstance();

		core::ECore()->Prepare();
		core::ECore()->SetFPSLimit(60);

		InitComponents();

		// TODO: �ӽ÷� �� ��. ���� main���� �̷� �������� �ű�鼭 ������ ����
		core::ECore()->mCameraGo->AddComponent<MoveComponent>();
		core::ECore()->mCameraGo->AddComponent<CameraComponent>();
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

		comManager->RegisterComponent<CameraComponent>();
		comManager->RegisterComponent<MoveComponent>();
	}

	////////////////////////////////
	//           WIN32            //
	////////////////////////////////

#ifdef _WIN32

}

#include "Win32/Win32Platform.h"

namespace cube{

	void CubeEngine::InitPlatform()
	{
		HINSTANCE ins;
		ins = GetModuleHandle(NULL);

		platform::Win32Platform::Init(ins);
	}

#endif // _WIN32

}
