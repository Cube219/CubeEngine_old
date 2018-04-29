#pragma once

#include "../EngineCoreHeader.h"

#include "../Resource/BaseResource.h"
#include "Vertex.h"

namespace cube
{
	namespace core
	{
		using Index = uint32_t;

		class ENGINE_CORE_EXPORT MeshImporter : public ResourceImporter
		{
		public:
			MeshImporter()
			{
				mName = CUBE_T("mesh");
			}

			Resource* Import(SPtr<platform::File>& file, Json info) final override;
		};

		class ENGINE_CORE_EXPORT Mesh : public Resource
		{
		public:
			Mesh(){ }
			virtual ~Mesh(){ }

			void SetVertex(Vector<Vertex>& vertices);
			void SetIndex(Vector<uint32_t>& indices);

			Vector<Vertex>& GetVertex() { return mVertices; }
			Vector<uint32_t>& GetIndex() { return mIndices; }

		private:
			friend class BaseMeshGenerator;

			Vector<Vertex> mVertices;
			Vector<uint32_t> mIndices;
		};
	} // namespace core
} // namespace cube
