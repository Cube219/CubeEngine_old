#include "CameraComponent.h"

#include "Base/MatrixUtility.h"

#include "EngineCore/GameObject.h"
#include "EngineCore/EngineCore.h"
#include "EngineCore/Renderer/RendererManager.h"
#include "EngineCore/LogWriter.h"

namespace cube
{
	String CameraComponent::mName = CUBE_T("CameraComponent");

	CameraComponent::CameraComponent()
	{
	}

	CameraComponent::~CameraComponent()
	{
	}

	void CameraComponent::OnInit()
	{
		mCameraRenderer3D = ECore().GetRendererManager().GetCameraRenderer3D();
	}

	void CameraComponent::OnUpdate(float dt)
	{
	}

	void CameraComponent::OnDestroy()
	{
	}
	
	void CameraComponent::OnTransformChanged()
	{
		Vector3 pos = GetGameObject()->GetPosition();

		Vector3 posForward = pos + GetGameObject()->GetForward();

		mViewMatrix = MatrixUtility::GetLookAt(
			pos, posForward, Vector3(0, 1, 0)
		);

		mCameraRenderer3D->SetViewMatrix(mViewMatrix);
		mCameraRenderer3D->SetPosition(pos);
	}
} // namespace cube
