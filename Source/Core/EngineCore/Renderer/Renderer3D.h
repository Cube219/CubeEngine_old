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
			void SetMaterial(SPtr<Material>& material);
			void SetModelMatrix(glm::mat4 modelMatrix);

			SPtr<BaseRenderDescriptorSet> GetDescriptorSet() const { return mDescriptorSet; };

			void Draw(SPtr<BaseRenderCommandBuffer>& commandBuffer, SPtr<CameraRenderer3D>& camera);

		private:
			void RecreateDescriptorSet();
			void RecreateDataBuffer();

			bool mIsMeshUpdated = false;
			SPtr<Mesh> mMesh;
			
			bool mIsMaterialUpdated = false;
			bool mIsMaterialDataUpdated = false;
			SPtr<Material> mMaterial;

			bool mIsMatrixUpdated = false;
			glm::mat4 mModelMatrix;
			SPtr<BaseRenderDescriptorSet> mDescriptorSet;

			SPtr<BaseRenderBuffer> mDataBuffer; // Combine vertex / index / mvp matrix / material parameter datas(except Texture)
			uint64_t mVertexIndex;
			uint64_t mIndexIndex;
			uint64_t mMVPIndex;
			uint64_t mMaterialParamIndex;

			SPtr<BaseRenderAPI> mRenderAPI_ref;
		};
	}
}