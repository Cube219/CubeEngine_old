#include "MoveComponent.h"

#include "EngineCore/ModuleManager.h"
#include "Base/format.h"
#include "EngineCore/LogWriter.h"

namespace cube
{
	String MoveComponent::mName = "MoveComponent";

	MoveComponent::MoveComponent()
	{
	}

	MoveComponent::~MoveComponent()
	{
	}

	void MoveComponent::OnInit()
	{
		String n = "InputModule";
		mInputModule = DPCast(module::InputModule)(core::ECore()->GetModuleManager()->GetModule(n));
	}

	void MoveComponent::OnUpdate()
	{
		String xStr = "MoveHorizontally";
		float x = mInputModule->GetAxisValue(xStr);
		String yStr = "MoveVertically";
		float y = mInputModule->GetAxisValue(yStr);

		CUBE_LOG(LogType::Info, fmt::format(L"Move: ({0}, {1})", x, y));
	}

	void MoveComponent::OnDestroy()
	{
	}
}
