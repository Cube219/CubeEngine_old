#include "Renderer3DComponent.h"

#include "EngineCore/EngineCore.h"
#include "EngineCore/Renderer/Renderer3D.h"
#include "EngineCore/Renderer/RendererManager.h"
//#include "EngineCore/Renderer/Mesh.h"
//#include "EngineCore/Renderer/Material/MaterialInstance.h"

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
		mRendererManager->RegisterRenderer3D(mRenderer3D);
	}

	void Renderer3DComponent::OnUpdate(float dt)
	{
	}

	void Renderer3DComponent::OnDestroy()
	{
		mRendererManager->UnregisterRenderer3D(mRenderer3D);
	}

	void Renderer3DComponent::SetMesh(SPtr<core::Mesh>& mesh)
	{
		mRenderer3D->SetMesh(mesh);
	}

	void Renderer3DComponent::SetMaterialInstance(SPtr<core::MaterialInstance>& materialIns)
	{
		mRenderer3D->SetMaterialInstance(materialIns);
	}
} // namespace cube
