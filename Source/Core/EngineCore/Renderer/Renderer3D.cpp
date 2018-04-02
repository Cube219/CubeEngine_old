#include "Renderer3D.h"

#include "Mesh.h"
#include "Material/Material.h"
#include "Material/MaterialInstance.h"
#include "Texture.h"
#include "Material/Shader.h"
#include "CameraRenderer3D.h"

#include <gtc/matrix_transform.hpp>

namespace cube
{
	namespace core
	{
		Renderer3D::Renderer3D(SPtr<BaseRenderAPI>& renderAPI, SPtr<BaseRenderDescriptorSetLayout>& mPerObjectDescriptorSetLayout) : 
			mRenderAPI_ref(renderAPI)
		{
			mDescriptorSet = renderAPI->CreateDescriptorSet(mPerObjectDescriptorSetLayout);
		}

		Renderer3D::~Renderer3D()
		{
		}

		void Renderer3D::SetMesh(SPtr<Mesh>& mesh)
		{
			mMesh = mesh;
			mIsMeshUpdated = true;
		}

		void Renderer3D::SetMaterialInstance(HMaterialInstance& materialIns)
		{
			mMaterialIns = materialIns;
		}

		void Renderer3D::SetModelMatrix(glm::mat4 modelMatrix)
		{
			mModelMatrix = modelMatrix;
		}

		void Renderer3D::Draw(SPtr<BaseRenderCommandBuffer>& commandBuffer, SPtr<CameraRenderer3D>& camera)
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
				mDataBuffer->Map(mMVPIndex, mMVPIndex);

				mIsMeshUpdated = false;
			}

			// Update mvp matrix
			auto mvpMatrix = camera->GetViewProjectionMatrix() * mModelMatrix;
			mDataBuffer->UpdateBufferData(mMVPIndex, &mvpMatrix, sizeof(mvpMatrix));

			BaseRenderBufferInfo bufInfo = mDataBuffer->GetInfo(mMVPIndex);
			mDescriptorSet->WriteBufferInDescriptor(0, 1, &bufInfo);

			// Write
			commandBuffer->BindDescriptorSets(PipelineType::Graphics, 1, 1, &mDescriptorSet);
			uint64_t vertexOffset = mDataBuffer->GetInfo(mVertexIndex).offset;
			commandBuffer->BindVertexBuffers(1, &mDataBuffer, &vertexOffset);
			commandBuffer->BindIndexBuffer(mDataBuffer, mDataBuffer->GetInfo(mIndexIndex).offset);

			commandBuffer->DrawIndexed(SCast(uint32_t)(indices.size()), 0, 0, 1, 0);
		}

		void Renderer3D::RecreateDataBuffer()
		{
			BaseRenderBufferInitializer init;
			init.type = BufferTypeBits::Uniform | BufferTypeBits::Vertex | BufferTypeBits::Index;

			BaseRenderBufferInitializer::BufferData bufData;
			bufData.data = nullptr;

			// 0. Vertex
			bufData.size = mMesh->GetVertex().size() * sizeof(Vertex);
			init.bufferDatas.push_back(bufData);
			mVertexIndex = 0;

			// 1. Index
			bufData.size = mMesh->GetIndex().size() * sizeof(Index);
			init.bufferDatas.push_back(bufData);
			mIndexIndex = 1;

			// 2. MVP matrix
			bufData.size = sizeof(mModelMatrix);
			init.bufferDatas.push_back(bufData);
			mMVPIndex = 2;

			mDataBuffer = mRenderAPI_ref->CreateBuffer(init);

			mDataBuffer->Map(mMVPIndex, mMVPIndex);
		}
	}
}