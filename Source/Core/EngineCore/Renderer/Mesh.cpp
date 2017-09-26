#include "Mesh.h"

namespace cube
{
	namespace core
	{
		Mesh::Mesh()
		{
		}

		Mesh::~Mesh()
		{
		}

		void Mesh::SetVertex(Vector<Vertex>& vertices)
		{
			mVertices = vertices;
		}

		void Mesh::SetIndex(Vector<uint32_t>& indices)
		{
			mIndices = indices;
		}
	}
}