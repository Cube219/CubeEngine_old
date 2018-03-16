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
		/*
		String leftTriggerStr = "TriggerLeft";
		float lt = mInputModule->GetAxisValue(leftTriggerStr);

		String rightTriggerStr = "TriggerRight";
		float rt = mInputModule->GetAxisValue(rightTriggerStr);

		mInputModule->SendVibration(0, 0.5f, lt, rt);*/

		//CUBE_LOG(LogType::Info, fmt::format(L"{0} / {1}", lt, rt));

		
		String submitStr = "Submit";
		bool r = mInputModule->IsActionPressed(submitStr);
		
		if(r == true) {
			mInputModule->SendVibration(0, 0.5f, 1.0f);
		}
		
		/*
		String xStr = "MoveHorizontally";
		float x = mInputModule->GetAxisValue(xStr);
		String yStr = "MoveVertically";
		float y = mInputModule->GetAxisValue(yStr);

		CUBE_LOG(LogType::Info, fmt::format(L"Move: ({0}, {1})", x, y));
		*/
	}

	void MoveComponent::OnDestroy()
	{
	}
}
