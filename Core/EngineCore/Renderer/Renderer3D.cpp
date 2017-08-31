#include "Renderer3D.h"

#include "CameraRenderer3D.h"

#include <gtc\matrix_transform.hpp>

namespace cube
{
	namespace core
	{
		Renderer3D::Renderer3D(SPtr<BaseRenderAPI>& renderAPI) : 
			mRenderAPI_ref(renderAPI)
		{
			mDescriptorSet = renderAPI->CreateDescriptorSet();
			mDescriptorSet->AddDescriptor(ShaderType::GLSL_Vertex, DescriptorType::UniformBuffer, 0, 1);
			mDescriptorSet->Create();
		}

		Renderer3D::~Renderer3D()
		{
		}

		void Renderer3D::SetVertex(const Vector<Vertex>& vertices)
		{
			mVertices = vertices;
			mIsVerticesUpdated = true;
		}

		void Renderer3D::SetIndex(const Vector<uint32_t>& indices)
		{
			mIndices = indices;
			mIsIndicesUpdated = true;
		}

		void Renderer3D::SetModelMatrix(glm::mat4 modelMatrix)
		{
			mModelMatrix = modelMatrix;
			mIsMatrixUpdated = true;
		}

		void Renderer3D::Draw(SPtr<BaseRenderCommandBuffer>& commandBuffer, SPtr<CameraRenderer3D>& camera)
		{
			if(mIsVerticesUpdated == true || mIsIndicesUpdated == true) {
				uint64_t dataSize = sizeof(mModelMatrix) + mVertices.size() * sizeof(Vertex) + mIndices.size() * sizeof(uint32_t);
				
				mUniformOffset = 0;
				mVertexOffset = sizeof(mModelMatrix);
				mIndexOffset = mVertexOffset + mVertices.size() * sizeof(Vertex);

				mDataBuffer = mRenderAPI_ref->CreateBuffer(dataSize,
					BufferTypeBits::Uniform | BufferTypeBits::Vertex | BufferTypeBits::Index);

				mDataBuffer->UpdateBufferData(mVertices.data(), mVertices.size() * sizeof(Vertex), mVertexOffset);
				mDataBuffer->UpdateBufferData(mIndices.data(), mIndices.size() * sizeof(uint32_t), mIndexOffset);

				mIsVerticesUpdated = false;
				mIsIndicesUpdated = false;

				// Uniform data must be written to the buffer because the buffer was newly created.
				mIsMatrixUpdated = true;
			}

			if(mIsMatrixUpdated == true) {
				auto mvpMatrix = camera->GetViewProjectionMatrix() * mModelMatrix;
				mDataBuffer->UpdateBufferData(&mvpMatrix, sizeof(mvpMatrix), mUniformOffset);

				BaseRenderBufferInfo bufInfo = mDataBuffer->GetInfo(mUniformOffset, sizeof(mModelMatrix));
				mDescriptorSet->WriteBufferInDescriptor(0, 1, &bufInfo);

				mIsMatrixUpdated = false;
			}

			// Write
			commandBuffer->BindDescriptorSets(PipelineType::Graphics, 0, 1, &mDescriptorSet);
			commandBuffer->BindVertexBuffers(1, &mDataBuffer, &mVertexOffset);
			commandBuffer->BindIndexBuffer(mDataBuffer, mIndexOffset);

			commandBuffer->DrawIndexed(SCast(uint32_t)(mIndices.size()), 0, 0, 1, 0);
		}
	}
}