#include "Renderer3D.h"

#include "../EngineCore.h"
#include "RendererManager.h"
#include "Base/MatrixUtility.h"
#include "Mesh.h"
#include "Material/Material.h"
#include "Material/MaterialInstance.h"
#include "Material/Shader.h"
#include "CameraRenderer3D.h"
#include "RenderAPI/Interface/Device.h"

namespace cube
{
	Renderer3D::Renderer3D()
	{
	}

	Renderer3D::~Renderer3D()
	{
	}

	UPtr<Renderer3D> Renderer3D::Create()
	{
		UPtr<Renderer3D> renderer3d(new Renderer3D());
		renderer3d->Initialize();

		return renderer3d;
	}

	SPtr<rt::RenderObject> Renderer3D::CreateRenderObject() const
	{
		SPtr<rt::Renderer3D> renderer3d_rt(new rt::Renderer3D());

		return renderer3d_rt;
	}

	void Renderer3D::SetMesh(RPtr<Mesh>& mesh)
	{
		mMesh = mesh;
		mMaterialInses.resize(mMesh->GetSubMeshes().size());
		QueueSyncTask([this]() {
			GetRenderObject()->SyncMesh(mMesh->GetRenderObject());
		});
	}

	void Renderer3D::SetMaterialInstance(HMaterialInstance& materialIns, Uint32 index)
	{
		mMaterialInses[index] = materialIns;
		QueueSyncTask([this, materialIns, index]() {
			GetRenderObject()->SyncMaterialInstance(materialIns->GetRenderObject(), index);
		});
	}

	void Renderer3D::SetModelMatrix(const Matrix& modelMatrix)
	{
		mModelMatrix = modelMatrix;
		QueueSyncTask([this]() {
			GetRenderObject()->SyncModelMatrix(mModelMatrix);
		});
	}

	namespace rt
	{
		Renderer3D::Renderer3D()
		{
		}

		void Renderer3D::Initialize()
		{
			RendererManager& rm = ECore().GetRendererManager();

			SPtr<render::ShaderParametersLayout> perObjectShaderParametersLayout = rm._GetPerObjectShaderParametersLayout();
			mShaderParameters = perObjectShaderParametersLayout->CreateParameters();
		}

		void Renderer3D::Destroy()
		{
			mMesh = nullptr;
			mMaterialInses.clear();
			mShaderParameters = nullptr;
		}

		void Renderer3D::SyncMesh(SPtr<Mesh>& mesh)
		{
			mMesh = mesh;
			mMaterialInses.resize(mMesh->GetSubMeshes().size());
			mIsMeshUpdated = true;
		}

		void Renderer3D::SyncMaterialInstance(SPtr<MaterialInstance>& materialIns, Uint32 index)
		{
			mMaterialInses[index] = materialIns;
		}

		void Renderer3D::SyncModelMatrix(const Matrix& modelMatrix)
		{
			mUBOPerObject.modelMatrix = modelMatrix;
		}

		void Renderer3D::PrepareDraw(SPtr<render::CommandList>& commandList, SPtr<rt::CameraRenderer3D>& camera)
		{
			// Update mvp matrix
			mUBOPerObject.mvp = mUBOPerObject.modelMatrix * camera->GetViewProjectionMatrix();

			mShaderParameters->UpdateParameter(0, &mUBOPerObject, sizeof(UBOPerObject));

			// Bind vertex / index data
			auto meshBuf = mMesh->GetMeshBuffer();
			Uint32 vertexOffset = SCast(Uint32)(mMesh->GetVertexOffset());
			commandList->BindVertexBuffers(0, 1, &meshBuf, &vertexOffset);
			commandList->BindIndexBuffer(meshBuf, SCast(Uint32)(mMesh->GetIndexOffset()));
		}
	} // namespace rt
} // namespace cube
