#include "CameraComponent.h"

#include <gtc/matrix_transform.hpp>

#include "EngineCore/GameObject.h"
#include "EngineCore/EngineCore.h"
#include "EngineCore/Renderer/RendererManager.h"
#include "Base/format.h"
#include "EngineCore/LogWriter.h"

namespace cube
{
	String CameraComponent::mName = "CameraComponent";

	CameraComponent::CameraComponent()
	{
	}

	CameraComponent::~CameraComponent()
	{
	}

	void CameraComponent::OnInit()
	{
		mCameraRenderer3D = core::ECore()->GetRendererManager()->GetCameraRenderer3D();
	}

	void CameraComponent::OnUpdate(float dt)
	{
		Vector3 pos = GetGameObject()->GetPosition();
		float posF[3];
		pos.GetFloat3(posF);

		Vector3 posForward = pos + GetGameObject()->GetForward();
		float posForwardF[3];
		posForward.GetFloat3(posForwardF);

		mViewMatrix = glm::lookAt(
			glm::vec3(posF[0], -posF[1], -posF[2]), // Flip y, z
			glm::vec3(posForwardF[0], -posForwardF[1], -posForwardF[2]), // Flip y, z
			glm::vec3(0, 1, 0)
		);

		mCameraRenderer3D->SetViewMatrix(mViewMatrix);
	}

	void CameraComponent::OnDestroy()
	{
	}
}
