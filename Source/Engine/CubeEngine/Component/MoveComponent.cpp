#include "MoveComponent.h"

#include "EngineCore/ModuleManager.h"
#include "EngineCore/LogWriter.h"
#include "EngineCore/GameObject.h"

namespace cube
{
	String MoveComponent::mName = CUBE_T("MoveComponent");

	MoveComponent::MoveComponent()
	{
	}

	MoveComponent::~MoveComponent()
	{
	}

	void MoveComponent::OnInit()
	{
		String n = CUBE_T("InputModule");
		mInputModule = DPCast(module::InputModule)(core::ECore()->GetModuleManager()->GetModule(n));

		mInputModule->LockCursor();
	}

	void MoveComponent::OnUpdate(float dt)
	{
		String xStr = CUBE_T("MoveHorizontally");
		float x = mInputModule->GetAxisValue(xStr);
		String yStr = CUBE_T("MoveVertically");
		float y = mInputModule->GetAxisValue(yStr);

		Vector3 pos = GetGameObject()->GetPosition();
		pos += GetGameObject()->GetForward() * y * 0.2f;
		pos += GetGameObject()->GetRight() * x * 0.2f;

		GetGameObject()->SetPosition(pos);

		String lookXStr = CUBE_T("LookHorizontally");
		float lookX = mInputModule->GetAxisValue(lookXStr);
		String lookYStr = CUBE_T("LookVertically");
		float lookY = mInputModule->GetAxisValue(lookYStr);

		Vector3 rot = GetGameObject()->GetRotation();

		rot += Vector3(0.0f, 1.0f, 0.0f) * lookX * 2.0f;
		rot += Vector3(1.0f, 0.0f, 0.0f) * lookY * 2.0f;

		GetGameObject()->SetRotation(rot);
	}

	void MoveComponent::OnDestroy()
	{
	}
} // namespace cube