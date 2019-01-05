#include "Mesh.h"

#include "../EngineCore.h"
#include "../Resource/ResourceManager.h"

namespace cube
{
	namespace core
	{
		RPtr<Mesh> Mesh::Load(StringRef path)
		{
			return ECore().GetResourceManager().LoadResource<Mesh>(path);
		}

		void Mesh::SetVertex(const Vector<Vertex>& vertices)
		{
			mVertices = vertices;
		}

		void Mesh::SetIndex(const Vector<Index>& indices)
		{
			mIndices = indices;
		}

		void Mesh::AddSubMesh(const SubMesh& subMesh)
		{
			mSubMeshes.push_back(subMesh);
		}
	} // namespace core
} // namespace cube
