#pragma once

#include "../EngineCoreHeader.h"

#include <glm.hpp>

#include "Vertex.h"
#include "../BasicHandler.h"
#include "BaseRenderAPI/Wrapper/BaseRenderBuffer.h"
#include "BaseRenderAPI/Wrapper/BaseRenderCommandBuffer.h"
#include "BaseRenderAPI/Wrapper/BaseRenderDescriptor.h"
#include "BaseRenderAPI/BaseRenderAPI.h"

namespace cube
{
	namespace core
	{
		class ENGINE_CORE_EXPORT Renderer3D
		{
		public:
			Renderer3D(SPtr<BaseRenderAPI>& renderAPI, SPtr<BaseRenderDescriptorSetLayout>& mPerObjectDescriptorSetLayout);
			~Renderer3D();

			HMaterialInstance GetMaterialInstance() const { return mMaterialIns; }

			SPtr<BaseRenderDescriptorSet> GetDescriptorSet() const { return mDescriptorSet; };

			void SetMesh(SPtr<Mesh>& mesh);
			void SetMaterialInstance(HMaterialInstance& materialIns);
			void SetModelMatrix(glm::mat4 modelMatrix);

			void Draw(SPtr<BaseRenderCommandBuffer>& commandBuffer, SPtr<CameraRenderer3D>& camera);

		private:
			friend class RendererManager;

			void RecreateDataBuffer();

			int mIndex = -1; // Used in RendererManager

			bool mIsMeshUpdated = false;
			SPtr<Mesh> mMesh;
			
			HMaterialInstance mMaterialIns;

			glm::mat4 mModelMatrix;
			SPtr<BaseRenderDescriptorSet> mDescriptorSet;

			SPtr<BaseRenderBuffer> mDataBuffer; // Combine vertex / index / mvp matrix
			uint64_t mVertexIndex;
			uint64_t mIndexIndex;
			uint64_t mMVPIndex;

			SPtr<BaseRenderAPI> mRenderAPI_ref;
		};
	} // namespace core
} // namespace cube
