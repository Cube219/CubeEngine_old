#include "ObjImporter.h"

#include <assimp/postprocess.h>

#include "EngineCore/Assertion.h"

namespace cube
{
	core::Resource* ObjImporter::Import(SPtr<platform::File>& file, Json info)
	{
		uint64_t size = file->GetFileSize();

		char* rawData = (char*)malloc(size);
		uint64_t readSize = 0;
		file->Read(rawData, size, readSize);

		const aiScene* scene = mImporter.ReadFileFromMemory(rawData, readSize, aiProcess_Triangulate | aiProcess_FlipUVs);
		CHECK(scene && scene->mRootNode, "Failed to import obj file.");

		core::Mesh* meshPtr = new core::Mesh();
		InsertMeshData(scene, meshPtr);

		mImporter.FreeScene();
		free(rawData);

		return meshPtr;
	}

	void ObjImporter::InsertMeshData(const aiScene* scene, core::Mesh* mesh)
	{
		Vector<core::Vertex>& meshVertices = mesh->GetVertex();
		Vector<core::Index>& meshIndices = mesh->GetIndex();

		// Reserve the size of vertices/indices for avoiding reallocation
		uint64_t vertexNum = 0, indexNum = 0;
		for(uint32_t i = 0; i < scene->mNumMeshes; i++) {
			aiMesh* aiMesh = scene->mMeshes[i];
			vertexNum += aiMesh->mNumVertices;
			indexNum += aiMesh->mNumFaces * aiMesh->mFaces[0].mNumIndices;
		}
		meshVertices.reserve(vertexNum);
		meshIndices.reserve(indexNum);

		for(uint32_t i = 0; i < scene->mNumMeshes; i++) {
			aiMesh* aiMesh = scene->mMeshes[i];

			core::SubMesh subMesh;
			subMesh.name = ToStringFromASCII(aiMesh->mName.C_Str());
			subMesh.vertexOffset = meshVertices.size();
			subMesh.indexOffset = meshIndices.size();
			subMesh.indexCount = aiMesh->mNumFaces * aiMesh->mFaces[0].mNumIndices;

			for(uint32_t j = 0; j < aiMesh->mNumVertices; j++) {
				core::Vertex v;
				v.pos = Vector4::Zero();
				v.normal = Vector3::Zero();
				v.texCoord = Vector2::Zero();
				
				if(aiMesh->HasPositions() == true) {
					aiVector3D position = aiMesh->mVertices[j];
					v.pos = Vector4(position.x, position.y, position.z, 1.0f);
				}

				if(aiMesh->HasNormals() == true) {
					aiVector3D normal = aiMesh->mNormals[j];
					v.normal = Vector3(normal.x, normal.y, normal.z);
				}

				if(aiMesh->HasTextureCoords(0) == true) {
					aiVector3D uv = aiMesh->mTextureCoords[0][j];
					v.texCoord = Vector2(uv.x, uv.y);
				}

				meshVertices.push_back(v);
			}
			
			for(uint32_t j = 0; j < aiMesh->mNumFaces; j++) {
				aiFace face = aiMesh->mFaces[j];

				for(uint32_t k = 0; k < face.mNumIndices; k++) {
					meshIndices.push_back(face.mIndices[k]);
				}
			}

			mesh->AddSubMesh(subMesh);
		}
	}
} // namesapce cube
