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
		String j = "Jump";
		bool r = mInputModule->IsActionPressed(j);

		CUBE_LOG(LogType::Info, fmt::format(L"Jump: {0}", r));
	}

	void MoveComponent::OnDestroy()
	{
	}
}
