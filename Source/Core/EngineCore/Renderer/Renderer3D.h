#pragma once

#include "../EngineCoreHeader.h"

#include <glm.hpp>

#include "Vertex.h"
#include "BaseRenderAPI/Wrapper/BaseRenderBuffer.h"
#include "BaseRenderAPI/Wrapper/BaseRenderCommandBuffer.h"
#include "BaseRenderAPI/Wrapper/BaseRenderDescriptor.h"
#include "BaseRenderAPI/BaseRenderAPI.h"

namespace cube
{
	namespace core
	{
		class Renderer3D
		{
		public:
			Renderer3D(SPtr<BaseRenderAPI>& renderAPI);
			~Renderer3D();

			void SetMesh(SPtr<Mesh>& mesh);
			void SetModelMatrix(glm::mat4 modelMatrix);
			void SetTexture(SPtr<Texture>& texture);

			void Draw(SPtr<BaseRenderCommandBuffer>& commandBuffer, SPtr<CameraRenderer3D>& camera);

			SPtr<BaseRenderBuffer> GetDataBuffer() const;
			uint64_t GetUniformBufferOffset() const;
			uint64_t GetUniformBufferSize() const;

			SPtr<BaseRenderDescriptorSet> GetDescriptorSet() const;

		private:
			bool mIsMeshUpdated = false;
			SPtr<Mesh> mMesh;
			
			bool mIsMatrixUpdated = false;
			glm::mat4 mModelMatrix;
			SPtr<BaseRenderDescriptorSet> mDescriptorSet; // TODO: 차후에 material쪽으로 옮김
			bool mIsTextureUpdated = false;
			SPtr<Texture> mTexture; // TODO: 차후에 material쪽으로 옮김

			SPtr<BaseRenderBuffer> mDataBuffer; // Combine uniform / vertex / index
			uint64_t mUniformOffset;
			uint64_t mVertexOffset;
			uint64_t mIndexOffset;

			SPtr<BaseRenderAPI> mRenderAPI_ref;
		};

		inline SPtr<BaseRenderBuffer> Renderer3D::GetDataBuffer() const
		{
			return mDataBuffer;
		}

		inline uint64_t Renderer3D::GetUniformBufferOffset() const
		{
			return mUniformOffset;
		}

		inline uint64_t Renderer3D::GetUniformBufferSize() const
		{
			return sizeof(mModelMatrix);
		}

		inline SPtr<BaseRenderDescriptorSet> Renderer3D::GetDescriptorSet() const
		{
			return mDescriptorSet;
		}
	}
}