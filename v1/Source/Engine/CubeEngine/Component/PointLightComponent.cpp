#include "PointLightComponent.h"

#include "EngineCore/EngineCore.h"
#include "EngineCore/Renderer/RendererManager.h"
#include "EngineCore/GameObject.h"

namespace cube
{
	String PointLightComponent::mName = CUBE_T("PointLightComponent");

	PointLightComponent::PointLightComponent()
	{
		mPointLight = ECore().GetRendererManager().RegisterLight(PointLight::Create());
	}

	PointLightComponent::~PointLightComponent()
	{
	}

	void PointLightComponent::OnInit()
	{
	}

	void PointLightComponent::OnUpdate(float dt)
	{
	}

	void PointLightComponent::OnDestroy()
	{
		ECore().GetRendererManager().UnregisterLight(mPointLight);
	}

	void PointLightComponent::OnTransformChanged()
	{
		mPointLight->SetPosition(GetGameObject()->GetPosition());
	}

	void PointLightComponent::SetColor(const Vector4& color)
	{
		mPointLight->SetColor(color);
	}
} // namespace cube
