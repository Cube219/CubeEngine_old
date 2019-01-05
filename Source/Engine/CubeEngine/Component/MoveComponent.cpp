#include "MoveComponent.h"

#include "../CubeEngine.h"
#include "EngineCore/EngineCore.h"
#include "EngineCore/Module/ModuleManager.h"
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
		mInputModule = DPCast(module::InputModule)(core::ECore().GetModuleManager().GetModule(n));

		mInputModule->LockCursor();
	}

	void MoveComponent::OnUpdate(float dt)
	{
		String xStr = CUBE_T("MoveHorizontally");
		float x = mInputModule->GetAxisValue(xStr);
		String yStr = CUBE_T("MoveVertically");
		float y = mInputModule->GetAxisValue(yStr);
		String upStr = CUBE_T("MoveUpDown");
		float u = mInputModule->GetAxisValue(upStr);

		Vector3 pos = GetGameObject()->GetPosition();
		pos += GetGameObject()->GetForward() * y * 0.2f;
		pos += GetGameObject()->GetRight() * x * 0.2f;
		pos += Vector3(0.0f, 1.0f, 0.0f) * u * 0.2f;

		GetGameObject()->SetPosition(pos);

		String lookXStr = CUBE_T("LookHorizontally");
		float lookX = mInputModule->GetAxisValue(lookXStr);
		String lookYStr = CUBE_T("LookVertically");
		float lookY = mInputModule->GetAxisValue(lookYStr);

		Vector3 rot = GetGameObject()->GetRotation();

		Float3 rotF3 = rot.GetFloat3();
		rotF3.y += lookX * 2.0f;
		rotF3.x -= lookY * 2.0f;

		if(rotF3.x >= 89.5f)
			rotF3.x = 89.5f;

		if(rotF3.x <= -89.5f)
			rotF3.x = -89.5f;

		rot = Vector3(rotF3.x, rotF3.y, rotF3.z);

		GetGameObject()->SetRotation(rot);

		String jumpStr = CUBE_T("Jump");
		bool b = mInputModule->IsActionPressed(jumpStr);
		if(b == true) {
			CubeEngine::Close();
		}
	}

	void MoveComponent::OnDestroy()
	{
	}

	void MoveComponent::OnTransformChanged()
	{
	}
} // namespace cube
