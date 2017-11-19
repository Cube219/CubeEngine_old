#include "Renderer3D.h"

#include "Mesh.h"
#include "Material/Material.h"
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

		void Renderer3D::SetMaterial(SPtr<Material>& material)
		{
			mMaterial = material;
			mMaterial->SetUpdateBool(&mIsMaterialDataUpdated);

			RecreateDescriptorSet();

			mIsMaterialUpdated = true;
		}

		void Renderer3D::SetModelMatrix(glm::mat4 modelMatrix)
		{
			mModelMatrix = modelMatrix;
			mIsMatrixUpdated = true;
		}

		void Renderer3D::Draw(SPtr<BaseRenderCommandBuffer>& commandBuffer, SPtr<CameraRenderer3D>& camera)
		{
			if(mMaterial == nullptr)
				return;

			auto vertices = mMesh->GetVertex();
			auto indices = mMesh->GetIndex();

			if(mIsMeshUpdated == true || mIsMaterialUpdated == true) {
				RecreateDataBuffer();
			}

			if(mIsMeshUpdated == true) {
				mDataBuffer->Map();

				mDataBuffer->UpdateBufferData(mVertexIndex, vertices.data(), vertices.size() * sizeof(Vertex));
				mDataBuffer->UpdateBufferData(mIndexIndex, indices.data(), indices.size() * sizeof(uint32_t));

				mIsMeshUpdated = false;

				// DescriptorSet data(modelMatrix + material) must be rewritten to the buffer because the buffer was newly created.
				mIsMatrixUpdated = true;
				mIsMaterialUpdated = true;
			}

			if(mIsMatrixUpdated == true) {
				mDataBuffer->Map();

				auto mvpMatrix = camera->GetViewProjectionMatrix() * mModelMatrix;
				mDataBuffer->UpdateBufferData(mMVPIndex, &mvpMatrix, sizeof(mvpMatrix));

				mDataBuffer->Unmap();

				BaseRenderBufferInfo bufInfo = mDataBuffer->GetInfo(mMVPIndex);
				mDescriptorSet->WriteBufferInDescriptor(0, 1, &bufInfo);

				mIsMatrixUpdated = false;
			}

			if(mIsMaterialDataUpdated == true) {
				mDataBuffer->Map();

				// Write all material parameters in descriptor
				auto params = mMaterial->GetParameters();
				
				uint64_t currentIndex = 0;
				for(size_t i = 0; i < params.size(); i++) {
					if(params[i].type == MaterialParameterType::Data) {
						mDataBuffer->UpdateBufferData(mMaterialParamIndex + currentIndex, params[i].data, params[i].size);

						BaseRenderBufferInfo bufInfo = mDataBuffer->GetInfo(mMaterialParamIndex + i);
						mDescriptorSet->WriteBufferInDescriptor((uint32_t)i + 1, 1, &bufInfo);

						currentIndex++;
					} else if(params[i].type == MaterialParameterType::Texture) {
						auto imageView = params[i].texture->GetImageView();
						auto sampler = params[i].texture->GetSampler();

						mDescriptorSet->WriteImagesInDescriptor((uint32_t)i + 1, 1, &imageView, &sampler);
					}
				}

				mDataBuffer->Unmap();

				mIsMaterialUpdated = false;
			}

			// Write
			commandBuffer->BindDescriptorSets(PipelineType::Graphics, 0, 1, &mDescriptorSet);
			uint64_t vertexOffset = mDataBuffer->GetInfo(mVertexIndex).offset;
			commandBuffer->BindVertexBuffers(1, &mDataBuffer, &vertexOffset);
			commandBuffer->BindIndexBuffer(mDataBuffer, mDataBuffer->GetInfo(mIndexIndex).offset);

			commandBuffer->DrawIndexed(SCast(uint32_t)(indices.size()), 0, 0, 1, 0);
		}

		void Renderer3D::RecreateDescriptorSet()
		{
			BaseRenderDescriptorSetInitializer init;
			BaseRenderDescriptorSetInitializer::Descriptor desc;

			// Basic MVP matrix
			desc.shaderType = ShaderType::GLSL_Vertex;
			desc.type = DescriptorType::UniformBuffer;
			desc.bindingIndex = 0;
			desc.count = 1;
			init.descriptors.push_back(desc);

			// Material parameters
			auto& params = mMaterial->GetParameters();
			for(size_t i = 0; i < params.size(); i++) {
				if(params[i].type == MaterialParameterType::Data) {
					desc.shaderType = ShaderType::GLSL_Vertex;
					desc.type = DescriptorType::UniformBuffer;
					desc.bindingIndex = (uint32_t)i + 1;
					desc.count = 1;
				} else if(params[i].type == MaterialParameterType::Texture) {
					desc.shaderType = ShaderType::GLSL_Fragment;
					desc.type = DescriptorType::CombinedImageSampler;
					desc.bindingIndex = (uint32_t)i + 1;
					desc.count = 1;
				} else {
					// TODO: 에러 처리
				}
				init.descriptors.push_back(desc);
			}

			mDescriptorSet = mRenderAPI_ref->CreateDescriptorSet(init);
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
			bufData.size = mMesh->GetIndex().size() * sizeof(uint32_t);
			init.bufferDatas.push_back(bufData);
			mIndexIndex = 1;

			// 2. MVP matrix
			bufData.size = sizeof(mModelMatrix);
			init.bufferDatas.push_back(bufData);
			mMVPIndex = 2;

			// 3 ~ . Material parameters
			auto& params = mMaterial->GetParameters();
			uint64_t paramNum = params.size();
			for(uint64_t i = 0; i < paramNum; i++) {
				// Texture data is not saved in data buffer
				if(params[i].type != MaterialParameterType::Texture) {
					bufData.size = params[i].size;
					init.bufferDatas.push_back(bufData);
				}
			}
			mMaterialParamIndex = 3;

			mDataBuffer = mRenderAPI_ref->CreateBuffer(init);
		}
	}
}