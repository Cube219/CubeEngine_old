#pragma once

#include "../EngineCoreHeader.h"

#include "../Resource/BaseResource.h"
#include "RenderObject.h"
#include "Vertex.h"
#include "RenderAPI/Interface/Buffer.h"

namespace cube
{
	using Index = Uint32;

	struct ENGINE_CORE_EXPORT SubMesh
	{
		String name;
		Uint64 vertexOffset;
		Uint64 indexOffset;
		Uint64 indexCount;
	};

	class ENGINE_CORE_EXPORT Mesh : public Resource, public RenderObject
	{
	public:
		static RPtr<Mesh> Load(StringRef path);

	public:
		Mesh();
		Mesh(const SPtr<MeshData>& initialiData);
		virtual ~Mesh();

		virtual SPtr<rt::RenderObject> CreateRenderObject() const override;
		SPtr<rt::Mesh> GetRenderObject() const { return DPCast(rt::Mesh)(mRenderObject); }

		const Vector<SubMesh>& GetSubMeshes() const { return mSubMeshes; }

	private:
		friend class BaseMeshGenerator;

		mutable SPtr<MeshData> mMeshData;
		Vector<SubMesh> mSubMeshes;
	};

	namespace rt
	{
		class Mesh : public RenderObject
		{
		public:
			Mesh(const SPtr<MeshData>& initialiData);
			virtual ~Mesh();
			virtual void Initialize() override;
			virtual void Destroy() override;

			void SyncMeshData(const SPtr<MeshData>& meshData);

			const Vector<SubMesh>& GetSubMeshes() const { return mSubMeshes; }
			SPtr<render::Buffer> GetMeshBuffer() const { return mMeshBuffer; }
			Uint64 GetVertexOffset() const { return mVertexOffset; }
			Uint64 GetIndexOffset() const { return mIndexOffset; }

		private:
			friend class cube::Mesh;

			void FlushToMeshBuffer();

			SPtr<MeshData> mMeshData;

			SPtr<render::Buffer> mMeshBuffer; // Combine vertex/index
			Uint64 mVertexOffset;
			Uint64 mIndexOffset;
			Vector<SubMesh> mSubMeshes;
		};
	} // namespace rt
} // namespace cube
