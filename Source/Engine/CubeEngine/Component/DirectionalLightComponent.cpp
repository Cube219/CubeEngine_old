#include "DirectionalLightComponent.h"

#include "EngineCore/EngineCore.h"
#include "EngineCore/Renderer/RendererManager.h"
#include "EngineCore/GameObject.h"

namespace cube
{
	String DirectionalLightComponent::mName = CUBE_T("DirectionalLightComponent");

	DirectionalLightComponent::DirectionalLightComponent()
	{
		mDirLight = std::make_shared<core::DirectionalLight>();
	}

	DirectionalLightComponent::~DirectionalLightComponent()
	{
	}

	void DirectionalLightComponent::OnInit()
	{
		core::ECore()->GetRendererManager()->RegisterLight(mDirLight);
	}

	void DirectionalLightComponent::OnUpdate(float dt)
	{
	}

	void DirectionalLightComponent::OnDestroy()
	{
		core::ECore()->GetRendererManager()->UnregisterLight(mDirLight);
	}

	void DirectionalLightComponent::OnTransformChanged()
	{
		mDirLight->SetDirection(GetGameObject()->GetForward());
	}

	void DirectionalLightComponent::SetDiffuse(const Vector4& diffuse)
	{
		mDirLight->SetDiffuse(diffuse);
	}
} // namespace cube
