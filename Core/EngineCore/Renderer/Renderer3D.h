#pragma once

#include "..\EngineCoreHeader.h"

#include <glm.hpp>

#include "Vertex.h"
#include "BaseRenderAPI\Wrapper\BaseRenderBuffer.h"
#include "BaseRenderAPI\Wrapper\BaseRenderCommandBuffer.h"
#include "BaseRenderAPI\Wrapper\BaseRenderDescriptor.h"
#include "BaseRenderAPI\BaseRenderAPI.h"

namespace cube
{
	namespace core
	{
		class Renderer3D
		{
		public:
			Renderer3D(SPtr<BaseRenderAPI>& renderAPI);
			~Renderer3D();

			void SetVertex(const Vector<Vertex>& vertices);
			void SetIndex(const Vector<uint32_t>& indices);
			void SetModelMatrix(glm::mat4 modelMatrix);

			void Draw(SPtr<BaseRenderCommandBuffer>& commandBuffer);

			SPtr<BaseRenderBuffer> GetDataBuffer() const;
			uint64_t GetUniformBufferOffset() const;
			uint64_t GetUniformBufferSize() const;
			uint64_t GetVertexBufferOffset() const;
			uint64_t GetVertexBufferSize() const;
			uint64_t GetIndexBufferOffset() const;
			uint64_t GetIndexBufferSize() const;

			SPtr<BaseRenderDescriptorSet> GetDescriptorSet() const;

		private:
			bool mIsVerticesUpdated = false;
			Vector<Vertex> mVertices;
			bool mIsIndicesUpdated = false;
			Vector<uint32_t> mIndices;
			
			bool mIsMatrixUpdated = false;
			glm::mat4 mModelMatrix;
			SPtr<BaseRenderDescriptorSet> mDescriptorSet; // TODO: 차후에 material쪽으로 옮김

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

		inline uint64_t Renderer3D::GetVertexBufferOffset() const
		{
			return mVertexOffset;
		}

		inline uint64_t Renderer3D::GetVertexBufferSize() const
		{
			return mVertices.size() * sizeof(Vertex);
		}

		inline uint64_t Renderer3D::GetIndexBufferOffset() const
		{
			return mIndexOffset;
		}

		inline uint64_t Renderer3D::GetIndexBufferSize() const
		{
			return mIndices.size() * sizeof(uint32_t);
		}

		inline SPtr<BaseRenderDescriptorSet> Renderer3D::GetDescriptorSet() const
		{
			return mDescriptorSet;
		}
	}
}