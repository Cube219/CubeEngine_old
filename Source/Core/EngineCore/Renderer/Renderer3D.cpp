#include "Renderer3D.h"

#include "../EngineCore.h"
#include "RendererManager.h"
#include "Base/MatrixUtility.h"
#include "Mesh.h"
#include "Material/Material.h"
#include "Material/MaterialInstance.h"
#include "Texture.h"
#include "Material/Shader.h"
#include "CameraRenderer3D.h"

namespace cube
{
	namespace core
	{
		Renderer3D::Renderer3D()
		{
		}

		Renderer3D::~Renderer3D()
		{
		}

		SPtr<Renderer3D> Renderer3D::Create()
		{
			SPtr<Renderer3D> renderer3d(new Renderer3D());
			renderer3d->Initialize();

			return renderer3d;
		}

		SPtr<RenderObject_RT> Renderer3D::CreateRenderObject_RT() const
		{
			SPtr<Renderer3D_RT> renderer3d_rt(new Renderer3D_RT());
			renderer3d_rt->Initialize();

			return renderer3d_rt;
		}

		void Renderer3D::SetMesh(RPtr<Mesh>& mesh)
		{
			mMesh = mesh;
			mMaterialInses.resize(mMesh->GetSubMeshes().size());
			QueueSyncTask([this]() {
				GetRenderObject_RT()->SyncMesh(mMesh);
			});
		}

		void Renderer3D::SetMaterialInstance(HMaterialInstance& materialIns, uint32_t index)
		{
			mMaterialInses[index] = materialIns;
			QueueSyncTask([this, materialIns, index]() {
				GetRenderObject_RT()->SyncMaterialInstance(materialIns, index);
			});
		}

		void Renderer3D::SetModelMatrix(const Matrix& modelMatrix)
		{
			mModelMatrix = modelMatrix;
			QueueSyncTask([this]() {
				GetRenderObject_RT()->SyncModelMatrix(mModelMatrix);
			});
		}

		Renderer3D_RT::Renderer3D_RT()
		{
			RendererManager& rm = ECore().GetRendererManager();
			mRenderAPI_ref = rm.GetRenderAPI();

			SPtr<render::DescriptorSetLayout> perObjectDescSetLayout = rm._GetPerObjectDescriptorSetLayout();
			mDescriptorSet = mRenderAPI_ref->CreateDescriptorSet(perObjectDescSetLayout);
		}

		void Renderer3D_RT::SyncMesh(RPtr<Mesh>& mesh)
		{
			mMesh = mesh;
			mMaterialInses.resize(mMesh->GetSubMeshes().size());
			mIsMeshUpdated = true;
		}

		void Renderer3D_RT::SyncMaterialInstance(HMaterialInstance materialIns, uint32_t index)
		{
			mMaterialInses[index] = materialIns->GetRenderObject_RT();
		}

		void Renderer3D_RT::SyncModelMatrix(const Matrix& modelMatrix)
		{
			mUBOPerObject.modelMatrix = modelMatrix;
		}

		void Renderer3D_RT::PrepareDraw(SPtr<render::CommandBuffer>& commandBuffer, SPtr<CameraRenderer3D_RT>& camera)
		{
			if(mIsMeshUpdated == true) {
				Vector<Vertex>& vertices = mMesh->GetVertex();
				Vector<Index>& indices = mMesh->GetIndex();

				RecreateDataBuffer();

				mDataBuffer->Unmap();
				mDataBuffer->Map(mVertexIndex, mIndexIndex);

				mDataBuffer->UpdateBufferData(mVertexIndex, vertices.data(), vertices.size() * sizeof(Vertex));
				mDataBuffer->UpdateBufferData(mIndexIndex, indices.data(), indices.size() * sizeof(Index));

				mDataBuffer->Unmap();
				mDataBuffer->Map(mUBOIndex, mUBOIndex);

				mIsMeshUpdated = false;
			}

			// Update mvp matrix
			mUBOPerObject.mvp = mUBOPerObject.modelMatrix * camera->GetViewProjectionMatrix();
			mDataBuffer->UpdateBufferData(mUBOIndex, &mUBOPerObject, sizeof(mUBOPerObject));

			render::BufferInfo bufInfo = mDataBuffer->GetInfo(mUBOIndex);
			mDescriptorSet->WriteBufferInDescriptor(0, 1, &bufInfo);

			// Bind vertex / index data
			uint64_t vertexOffset = mDataBuffer->GetInfo(mVertexIndex).offset;
			commandBuffer->BindVertexBuffers(1, &mDataBuffer, &vertexOffset);
			commandBuffer->BindIndexBuffer(mDataBuffer, mDataBuffer->GetInfo(mIndexIndex).offset);
		}

		void Renderer3D_RT::RecreateDataBuffer()
		{
			render::BufferInitializer init;
			init.type = render::BufferTypeBits::Uniform | render::BufferTypeBits::Vertex | render::BufferTypeBits::Index;

			render::BufferInitializer::BufferData bufData;
			bufData.data = nullptr;

			// 0. Vertex
			bufData.size = mMesh->GetVertex().size() * sizeof(Vertex);
			init.bufferDatas.push_back(bufData);
			mVertexIndex = 0;

			// 1. Index
			bufData.size = mMesh->GetIndex().size() * sizeof(Index);
			init.bufferDatas.push_back(bufData);
			mIndexIndex = 1;

			// 2. UBO
			bufData.size = sizeof(UBOPerObject);
			init.bufferDatas.push_back(bufData);
			mUBOIndex = 2;

			mDataBuffer = mRenderAPI_ref->CreateBuffer(init);

			mDataBuffer->Map(mUBOIndex, mUBOIndex);
		}
	} // namespace core
} // namespace cube
