#pragma once

#include "../EngineCoreHeader.h"

#include "Vertex.h"

namespace cube
{
	namespace core
	{
		using Index = uint32_t;

		class ENGINE_CORE_EXPORT Mesh
		{
		public:
			Mesh();
			~Mesh();

			void SetVertex(Vector<Vertex>& vertices);
			void SetIndex(Vector<uint32_t>& indices);

			Vector<Vertex>& GetVertex() { return mVertices; }
			Vector<uint32_t>& GetIndex() { return mIndices; }

		private:
			Vector<Vertex> mVertices;
			Vector<uint32_t> mIndices;
		};
	}
}