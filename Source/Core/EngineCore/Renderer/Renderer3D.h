#pragma once

#include "../EngineCoreHeader.h"

#include "RenderObject.h"
#include "Vertex.h"
#include "Base/Matrix.h"
#include "../BasicHandler.h"
#include "RenderAPI/Interface/ShaderParameters.h"
#include "RenderAPI/Interface/Device.h"
#include "RenderAPI/Interface/Buffer.h"
#include "RenderAPI/Interface/CommandList.h"

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

			SPtr<render::ShaderParameters> GetShaderParameters() const { return mShaderParameters; };

			void PrepareDraw(SPtr<render::CommandList>& commandList, SPtr<CameraRenderer3D_RT>& camera);

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
			SPtr<render::ShaderParameters> mShaderParameters;

			SPtr<render::Buffer> mDataBuffer; // Combine vertex / index
			void* mDataBufferMappedPtr;
			Uint32 mVertexOffset;
			Uint32 mIndexOffset;

			SPtr<render::Device> mDevice;
		};
	} // namespace core
} // namespace cube
