#include "Renderer3D.h"

#include "Mesh.h"
#include "Texture.h"
#include "CameraRenderer3D.h"

#include <gtc/matrix_transform.hpp>

namespace cube
{
	namespace core
	{
		Renderer3D::Renderer3D(SPtr<BaseRenderAPI>& renderAPI) : 
			mRenderAPI_ref(renderAPI)
		{
			BaseRenderDescriptorSetInitializer descriptorSetInitializer;
			descriptorSetInitializer.descriptors.push_back({ShaderType::GLSL_Vertex, DescriptorType::UniformBuffer, 0, 1});
			descriptorSetInitializer.descriptors.push_back({ShaderType::GLSL_Fragment, DescriptorType::CombinedImageSampler, 1, 1});
			
			mDescriptorSet = renderAPI->CreateDescriptorSet(descriptorSetInitializer);
		}

		Renderer3D::~Renderer3D()
		{
		}

		void Renderer3D::SetMesh(SPtr<Mesh>& mesh)
		{
			mMesh = mesh;
			mIsMeshUpdated = true;
		}

		void Renderer3D::SetModelMatrix(glm::mat4 modelMatrix)
		{
			mModelMatrix = modelMatrix;
			mIsMatrixUpdated = true;
		}

		void Renderer3D::SetTexture(SPtr<Texture>& texture)
		{
			mTexture = texture;
			mIsTextureUpdated = true;
		}

		void Renderer3D::Draw(SPtr<BaseRenderCommandBuffer>& commandBuffer, SPtr<CameraRenderer3D>& camera)
		{
			auto vertices = mMesh->GetVertex();
			auto indices = mMesh->GetIndex();

			if(mIsMeshUpdated == true) {
				uint64_t dataSize = sizeof(mModelMatrix) + vertices.size() * sizeof(Vertex) + indices.size() * sizeof(uint32_t);
				
				mUniformOffset = 0;
				mVertexOffset = sizeof(mModelMatrix);
				mIndexOffset = mVertexOffset + vertices.size() * sizeof(Vertex);

				mDataBuffer = mRenderAPI_ref->CreateBuffer(dataSize,
					BufferTypeBits::Uniform | BufferTypeBits::Vertex | BufferTypeBits::Index);

				mDataBuffer->Map();

				mDataBuffer->UpdateBufferData(vertices.data(), vertices.size() * sizeof(Vertex), mVertexOffset);
				mDataBuffer->UpdateBufferData(indices.data(), indices.size() * sizeof(uint32_t), mIndexOffset);

				mIsMeshUpdated = false;

				// Uniform data must be written to the buffer because the buffer was newly created.
				mIsMatrixUpdated = true;
			}

			if(mIsMatrixUpdated == true) {
				mDataBuffer->Map();

				auto mvpMatrix = camera->GetViewProjectionMatrix() * mModelMatrix;
				mDataBuffer->UpdateBufferData(&mvpMatrix, sizeof(mvpMatrix), mUniformOffset);

				mDataBuffer->Unmap();

				BaseRenderBufferInfo bufInfo = mDataBuffer->GetInfo(mUniformOffset, sizeof(mModelMatrix));
				mDescriptorSet->WriteBufferInDescriptor(0, 1, &bufInfo);

				mIsMatrixUpdated = false;
			}

			if(mIsTextureUpdated == true) {
				auto imageView = mTexture->GetImageView();
				auto sampler = mTexture->GetSampler();

				mDescriptorSet->WriteImagesInDescriptor(1, 1, &imageView, &sampler);

				mIsTextureUpdated = false;
			}

			// Write
			commandBuffer->BindDescriptorSets(PipelineType::Graphics, 0, 1, &mDescriptorSet);
			commandBuffer->BindVertexBuffers(1, &mDataBuffer, &mVertexOffset);
			commandBuffer->BindIndexBuffer(mDataBuffer, mIndexOffset);

			commandBuffer->DrawIndexed(SCast(uint32_t)(indices.size()), 0, 0, 1, 0);
		}
	}
}