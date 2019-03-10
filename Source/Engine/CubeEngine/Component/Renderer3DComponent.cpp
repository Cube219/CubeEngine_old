#include "Renderer3DComponent.h"

#include "EngineCore/EngineCore.h"
#include "EngineCore/Renderer/Renderer3D.h"
#include "EngineCore/Renderer/RendererManager.h"
#include "EngineCore/GameObject.h"

namespace cube
{
	String Renderer3DComponent::mName = CUBE_T("Renderer3DComponent");

	Renderer3DComponent::Renderer3DComponent() : 
		mRendererManager(ECore().GetRendererManager())
	{
		mRenderer3D = mRendererManager.RegisterRenderer3D(Renderer3D::Create());
	}

	Renderer3DComponent::~Renderer3DComponent()
	{
	}

	void Renderer3DComponent::OnInit()
	{
		mRenderer3D->SetModelMatrix(GetGameObject()->GetModelMatrix());
	}

	void Renderer3DComponent::OnUpdate(float dt)
	{
	}

	void Renderer3DComponent::OnDestroy()
	{
		mRendererManager.UnregisterRenderer3D(mRenderer3D);
	}

	void Renderer3DComponent::OnTransformChanged()
	{
		mRenderer3D->SetModelMatrix(GetGameObject()->GetModelMatrix());
	}

	void Renderer3DComponent::SetMesh(RPtr<Mesh>& mesh)
	{
		mRenderer3D->SetMesh(mesh);
	}

	void Renderer3DComponent::SetMaterialInstance(HMaterialInstance& materialIns, uint32_t index)
	{
		mRenderer3D->SetMaterialInstance(materialIns, index);
	}
} // namespace cube
