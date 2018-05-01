#pragma once

#include "../EngineCoreHeader.h"

#include "../Resource/BaseResource.h"
#include "Vertex.h"

namespace cube
{
	namespace core
	{
		using Index = uint32_t;

		struct ENGINE_CORE_EXPORT SubMesh
		{
			String name;
			uint64_t vertexOffset;
			uint64_t indexOffset;
			uint64_t indexCount;
		};

		class ENGINE_CORE_EXPORT Mesh : public Resource
		{
		public:
			static RPtr<Mesh> Load(const String& path);

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
	} // namespace core
} // namespace cube
