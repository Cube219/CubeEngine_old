#include "DirectionalLightComponent.h"

#include "EngineCore/EngineCore.h"
#include "EngineCore/Renderer/RendererManager.h"
#include "EngineCore/GameObject.h"

namespace cube
{
	String DirectionalLightComponent::mName = CUBE_T("DirectionalLightComponent");

	DirectionalLightComponent::DirectionalLightComponent()
	{
		mDirLight = ECore().GetRendererManager().RegisterLight(DirectionalLight::Create());
	}

	DirectionalLightComponent::~DirectionalLightComponent()
	{
	}

	void DirectionalLightComponent::OnInit()
	{
	}

	void DirectionalLightComponent::OnUpdate(float dt)
	{
		/*Vector3 rot = GetGameObject()->GetRotation();
		rot += Vector3(1, 0, 0) * 50.0f * dt;
		GetGameObject()->SetRotation(rot);*/
	}

	void DirectionalLightComponent::OnDestroy()
	{
		ECore().GetRendererManager().UnregisterLight(mDirLight);
	}

	void DirectionalLightComponent::OnTransformChanged()
	{
		mDirLight->SetDirection(GetGameObject()->GetForward());
	}

	void DirectionalLightComponent::SetColor(const Vector4& color)
	{
		mDirLight->SetColor(color);
	}
} // namespace cube
