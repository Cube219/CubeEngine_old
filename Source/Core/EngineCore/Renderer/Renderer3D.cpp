#include "Renderer3D.h"

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
		Renderer3D::Renderer3D(SPtr<render::RenderAPI>& renderAPI, SPtr<render::DescriptorSetLayout>& mPerObjectDescriptorSetLayout) : 
			mRenderAPI_ref(renderAPI)
		{
			mDescriptorSet = renderAPI->CreateDescriptorSet(mPerObjectDescriptorSetLayout);
		}

		Renderer3D::~Renderer3D()
		{
		}

		void Renderer3D::SetMesh(RPtr<Mesh>& mesh)
		{
			mMesh = mesh;
			mIsMeshUpdated = true;
		}

		void Renderer3D::SetMaterialInstance(HMaterialInstance& materialIns)
		{
			mMaterialIns = materialIns;
		}

		void Renderer3D::SetModelMatrix(const Matrix& modelMatrix)
		{
			mUBOPerObject.modelMatrix = modelMatrix;
		}

		void Renderer3D::Draw(SPtr<render::CommandBuffer>& commandBuffer, SPtr<CameraRenderer3D>& camera)
		{
			if(mMaterialIns.IsDestroyed() == true)
				return;

			Vector<Vertex>& vertices = mMesh->GetVertex();
			Vector<Index>& indices = mMesh->GetIndex();

			if(mIsMeshUpdated == true) {
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

			// Write
			commandBuffer->BindDescriptorSets(render::PipelineType::Graphics, 2, 1, &mDescriptorSet);
			uint64_t vertexOffset = mDataBuffer->GetInfo(mVertexIndex).offset;
			commandBuffer->BindVertexBuffers(1, &mDataBuffer, &vertexOffset);
			commandBuffer->BindIndexBuffer(mDataBuffer, mDataBuffer->GetInfo(mIndexIndex).offset);

			commandBuffer->DrawIndexed(SCast(uint32_t)(indices.size()), 0, 0, 1, 0);
		}

		void Renderer3D::RecreateDataBuffer()
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
