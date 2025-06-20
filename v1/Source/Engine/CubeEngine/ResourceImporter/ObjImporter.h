#pragma once

#include "../CubeEngineHeader.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include "EngineCore/Resource/BaseResource.h"
#include "EngineCore/Renderer/Mesh.h"

namespace cube
{
	class ObjImporter : public ResourceImporter
	{
	public:
		ObjImporter()
		{
			mName = CUBE_T("ObjImporter");
		}

		Resource* Import(SPtr<platform::File>& file, Json info) final override;

	private:
		SPtr<MeshData> GetMeshData(const aiScene* scene);

		Assimp::Importer mImporter;
	};
} // namespace cube
