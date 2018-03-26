#include "CameraComponent.h"

#include <gtc/matrix_transform.hpp>

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
		mCameraRenderer3D = core::ECore()->GetRendererManager()->GetCameraRenderer3D();
	}

	void CameraComponent::OnUpdate(float dt)
	{
		Vector3 pos = GetGameObject()->GetPosition();
		Float3 posF;
		posF = pos.GetFloat3();

		Vector3 posForward = pos + GetGameObject()->GetForward();
		Float3 posForwardF;
		posForwardF = posForward.GetFloat3();

		mViewMatrix = glm::lookAt(
			glm::vec3(posF.x, -posF.y, -posF.z), // Flip y, z
			glm::vec3(posForwardF.x, -posForwardF.y, -posForwardF.z), // Flip y, z
			glm::vec3(0, 1, 0)
		);

		mCameraRenderer3D->SetViewMatrix(mViewMatrix);
	}

	void CameraComponent::OnDestroy()
	{
	}
} // namespace cube
