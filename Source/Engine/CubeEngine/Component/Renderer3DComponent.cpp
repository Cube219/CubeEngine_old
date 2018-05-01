#include "Renderer3DComponent.h"

#include "EngineCore/EngineCore.h"
#include "EngineCore/Renderer/Renderer3D.h"
#include "EngineCore/Renderer/RendererManager.h"
#include "EngineCore/GameObject.h"

namespace cube
{
	String Renderer3DComponent::mName = CUBE_T("Renderer3DComponent");

	Renderer3DComponent::Renderer3DComponent()
	{
		mRendererManager = core::ECore()->GetRendererManager();

		mRenderer3D = mRendererManager->CreateRenderer3D();
	}

	Renderer3DComponent::~Renderer3DComponent()
	{
	}

	void Renderer3DComponent::OnInit()
	{
		GetGameObject()->mRenderer3D = mRenderer3D;
		mRendererManager->RegisterRenderer3D(mRenderer3D);

		GetGameObject()->mIsTransformChanged = true; // Update model matrix
	}

	void Renderer3DComponent::OnUpdate(float dt)
	{
	}

	void Renderer3DComponent::OnDestroy()
	{
		GetGameObject()->mRenderer3D = nullptr;
		mRendererManager->UnregisterRenderer3D(mRenderer3D);
	}

	void Renderer3DComponent::OnTransformChanged()
	{
	}

	void Renderer3DComponent::SetMesh(core::RPtr<core::Mesh>& mesh)
	{
		mRenderer3D->SetMesh(mesh);
	}

	void Renderer3DComponent::SetMaterialInstance(core::HMaterialInstance& materialIns, uint32_t index)
	{
		mRenderer3D->SetMaterialInstance(materialIns, index);
	}
} // namespace cube
