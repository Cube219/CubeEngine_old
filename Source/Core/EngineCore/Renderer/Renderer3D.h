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

		virtual SPtr<rt::RenderObject> CreateRenderObject() const override;
		SPtr<rt::Renderer3D> GetRenderObject() const { return DPCast(rt::Renderer3D)(mRenderObject); }

		void SetMesh(RPtr<Mesh>& mesh);
		void SetMaterialInstance(HMaterialInstance& materialIns, Uint32 index);
		void SetModelMatrix(const Matrix& modelMatrix);

	private:
		Renderer3D();

		RPtr<Mesh> mMesh;
			
		Vector<HMaterialInstance> mMaterialInses;

		Matrix mModelMatrix;
	};

	namespace rt
	{
		class Renderer3D : public RenderObject
		{
		public:
			virtual ~Renderer3D() {}
			virtual void Initialize() override;
			virtual void Destroy() override;

			void SyncMesh(RPtr<Mesh>& mesh);
			void SyncMaterialInstance(HMaterialInstance materialIns, Uint32 index);
			void SyncModelMatrix(const Matrix& modelMatrix);

			SPtr<render::ShaderParameters> GetShaderParameters() const { return mShaderParameters; };

			void PrepareDraw(SPtr<render::CommandList>& commandList, SPtr<rt::CameraRenderer3D>& camera);

		private:
			friend class cube::Renderer3D;
			friend class RendererManager;

			Renderer3D();

			void RecreateDataBuffer();

			int mIndex = -1; // Used in RendererManager

			bool mIsMeshUpdated = false;
			RPtr<Mesh> mMesh;

			Vector<SPtr<rt::MaterialInstance>> mMaterialInses;

			UBOPerObject mUBOPerObject;
			SPtr<render::ShaderParameters> mShaderParameters;

			SPtr<render::Buffer> mDataBuffer; // Combine vertex / index
			void* mDataBufferMappedPtr;
			Uint32 mVertexOffset;
			Uint32 mIndexOffset;
		};
	} // namespace rt
} // namespace cube
