#pragma once

#include "../EngineCoreHeader.h"

#include "RenderObject.h"
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
		struct UBOPerObject
		{
			Matrix mvp;
			Matrix modelMatrix;
		};

		class ENGINE_CORE_EXPORT Renderer3D : public RenderObject
		{
		public:
			virtual ~Renderer3D();

			static SPtr<Renderer3D> Create();

			virtual SPtr<RenderObject_RT> CreateRenderObject_RT() const override;
			SPtr<Renderer3D_RT> GetRenderObject_RT() const { return DPCast(Renderer3D_RT)(mRenderObject_RT); }

			void SetMesh(RPtr<Mesh>& mesh);
			void SetMaterialInstance(HMaterialInstance& materialIns, uint32_t index);
			void SetModelMatrix(const Matrix& modelMatrix);

		private:
			Renderer3D();

			RPtr<Mesh> mMesh;
			
			Vector<HMaterialInstance> mMaterialInses;

			Matrix mModelMatrix;
		};

		class Renderer3D_RT : public RenderObject_RT
		{
		public:
			virtual ~Renderer3D_RT(){ }

			void SyncMesh(RPtr<Mesh>& mesh);
			void SyncMaterialInstance(HMaterialInstance materialIns, uint32_t index);
			void SyncModelMatrix(const Matrix& modelMatrix);

			SPtr<render::DescriptorSet> GetDescriptorSet() const { return mDescriptorSet; };

			void PrepareDraw(SPtr<render::CommandBuffer>& commandBuffer, SPtr<CameraRenderer3D_RT>& camera);

		private:
			friend class Renderer3D;
			friend class RendererManager;

			Renderer3D_RT();

			void RecreateDataBuffer();

			int mIndex = -1; // Used in RendererManager

			bool mIsMeshUpdated = false;
			RPtr<Mesh> mMesh;

			Vector<SPtr<MaterialInstance_RT>> mMaterialInses;

			UBOPerObject mUBOPerObject;
			SPtr<render::DescriptorSet> mDescriptorSet;

			SPtr<render::Buffer> mDataBuffer; // Combine vertex / index / uboPerObject data
			uint64_t mVertexIndex;
			uint64_t mIndexIndex;
			uint64_t mUBOIndex;

			SPtr<render::RenderAPI> mRenderAPI_ref;
		};
	} // namespace core
} // namespace cube
