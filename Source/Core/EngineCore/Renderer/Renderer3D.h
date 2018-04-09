#pragma once

#include "../EngineCoreHeader.h"

#include "Vertex.h"
#include "Base/Matrix.h"
#include "../BasicHandler.h"
#include "BaseRenderAPI/Wrapper/Buffer.h"
#include "BaseRenderAPI/Wrapper/CommandBuffer.h"
#include "BaseRenderAPI/Wrapper/Descriptor.h"
#include "BaseRenderAPI/RenderAPI.h"

namespace cube
{
	namespace core
	{
		class ENGINE_CORE_EXPORT Renderer3D
		{
		public:
			Renderer3D(SPtr<render::RenderAPI>& renderAPI, SPtr<render::DescriptorSetLayout>& mPerObjectDescriptorSetLayout);
			~Renderer3D();

			HMaterialInstance GetMaterialInstance() const { return mMaterialIns; }

			SPtr<render::DescriptorSet> GetDescriptorSet() const { return mDescriptorSet; };

			void SetMesh(SPtr<Mesh>& mesh);
			void SetMaterialInstance(HMaterialInstance& materialIns);
			void SetModelMatrix(const Matrix& modelMatrix);

			void Draw(SPtr<render::CommandBuffer>& commandBuffer, SPtr<CameraRenderer3D>& camera);

		private:
			friend class RendererManager;

			void RecreateDataBuffer();

			int mIndex = -1; // Used in RendererManager

			bool mIsMeshUpdated = false;
			SPtr<Mesh> mMesh;
			
			HMaterialInstance mMaterialIns;

			Matrix mModelMatrix;
			SPtr<render::DescriptorSet> mDescriptorSet;

			SPtr<render::Buffer> mDataBuffer; // Combine vertex / index / mvp matrix
			uint64_t mVertexIndex;
			uint64_t mIndexIndex;
			uint64_t mMVPIndex;

			SPtr<render::RenderAPI> mRenderAPI_ref;
		};
	} // namespace core
} // namespace cube
