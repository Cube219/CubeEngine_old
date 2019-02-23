#pragma once

#include "../EngineCoreHeader.h"

#include "../Resource/BaseResource.h"
#include "Vertex.h"

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

	class ENGINE_CORE_EXPORT Mesh : public Resource
	{
	public:
		static RPtr<Mesh> Load(StringRef path);

	public:
		Mesh(){ }
		virtual ~Mesh(){ }

		void SetVertex(const Vector<Vertex>& vertices);
		void SetIndex(const Vector<Index>& indices);
		void AddSubMesh(const SubMesh& subMesh);

		Vector<Vertex>& GetVertex() { return mVertices; }
		Vector<Index>& GetIndex() { return mIndices; }
		Vector<SubMesh>& GetSubMeshes() { return mSubMeshes; }

	private:
		friend class BaseMeshGenerator;

		Vector<Vertex> mVertices;
		Vector<Index> mIndices;

		Vector<SubMesh> mSubMeshes;
	};
} // namespace cube
