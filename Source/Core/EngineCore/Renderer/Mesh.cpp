#include "Mesh.h"

#include "../LogWriter.h"

namespace cube
{
	namespace core
	{
		Resource* MeshImporter::Import(SPtr<platform::File>& file, Json info)
		{
			CUBE_LOG(LogType::Error, "Mesh can not be imported into the importer. Use BaseMeshGenerator instead.");
			
			return nullptr;
		}

		void Mesh::SetVertex(Vector<Vertex>& vertices)
		{
			mVertices = vertices;
		}

		void Mesh::SetIndex(Vector<uint32_t>& indices)
		{
			mIndices = indices;
		}
	} // namespace core
} // namespace cube
