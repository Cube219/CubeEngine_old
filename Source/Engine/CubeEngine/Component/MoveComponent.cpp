#include "MoveComponent.h"

#include "EngineCore/ModuleManager.h"
#include "Base/format.h"
#include "EngineCore/LogWriter.h"

namespace cube
{
	MoveComponent::MoveComponent()
	{
	}

	MoveComponent::~MoveComponent()
	{
	}

	void MoveComponent::OnInit()
	{
		//mInputModule = DPCast(module::InputModule)(ECore()->GetModuleManager()->GetModule(String("InputModule")));
	}

	void MoveComponent::OnUpdate()
	{
		//bool r = mInputModule->IsActionPressed(String("Jump"));

		//CUBE_LOG(LogType::Info, fmt::format(L"Jump: {0}", r));
	}

	void MoveComponent::OnDestroy()
	{
	}
}