#pragma once

#include "../CubeEngineHeader.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include "EngineCore/Resource/BaseResource.h"
#include "EngineCore/Renderer/Mesh.h"

namespace cube
{
	class ObjImporter : public core::ResourceImporter
	{
	public:
		ObjImporter()
		{
			mName = CUBE_T("ObjImporter");
		}

		core::Resource* Import(SPtr<platform::File>& file, Json info) final override;

	private:
		void InsertMeshData(const aiScene* scene, core::Mesh* mesh);

		Assimp::Importer mImporter;
	};
} // namespace cube
