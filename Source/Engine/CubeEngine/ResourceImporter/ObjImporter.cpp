#include "ObjImporter.h"

#include <assimp/postprocess.h>

#include "EngineCore/Assertion.h"
#include "EngineCore/Renderer/MeshData.h"

namespace cube
{
	Resource* ObjImporter::Import(SPtr<platform::File>& file, Json info)
	{
		uint64_t size = file->GetFileSize();

		char* rawData = (char*)malloc(size);
		uint64_t readSize = 0;
		file->Read(rawData, size, readSize);

		const aiScene* scene = mImporter.ReadFileFromMemory(rawData, readSize, aiProcess_Triangulate | aiProcess_FlipUVs);
		CHECK(scene && scene->mRootNode, "Failed to import obj file.");

		SPtr<MeshData> meshData = GetMeshData(scene);
		Mesh* meshPtr = new Mesh(meshData);

		mImporter.FreeScene();
		free(rawData);

		return meshPtr;
	}

	SPtr<MeshData> ObjImporter::GetMeshData(const aiScene* scene)
	{
		Uint64 vertexNum = 0, indexNum = 0;
		for(uint32_t i = 0; i < scene->mNumMeshes; i++) {
			aiMesh* aiMesh = scene->mMeshes[i];
			vertexNum += aiMesh->mNumVertices;
			indexNum += aiMesh->mNumFaces * aiMesh->mFaces[0].mNumIndices;
		}

		Vector<Vertex> vertices;
		Vector<Index> indices;
		Vector<SubMesh> subMeshes;

		vertices.reserve(vertexNum);
		indices.reserve(indexNum);

		for(uint32_t i = 0; i < scene->mNumMeshes; i++) {
			aiMesh* aiMesh = scene->mMeshes[i];

			SubMesh subMesh;
			subMesh.name = ToStringFromASCII(aiMesh->mName.C_Str());
			subMesh.vertexOffset = vertices.size();
			subMesh.indexOffset = indices.size();
			subMesh.indexCount = aiMesh->mNumFaces * aiMesh->mFaces[0].mNumIndices;

			for(uint32_t j = 0; j < aiMesh->mNumVertices; j++) {
				Vertex v;
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

				vertices.push_back(v);
			}

			for(uint32_t j = 0; j < aiMesh->mNumFaces; j++) {
				aiFace face = aiMesh->mFaces[j];

				for(uint32_t k = 0; k < face.mNumIndices; k++) {
					indices.push_back(face.mIndices[k]);
				}
			}

			subMeshes.push_back(subMesh);
		}

		SPtr<MeshData> meshData = std::make_shared<MeshData>(vertexNum, indexNum);
		meshData->SetVertices(vertices.data(), vertexNum);
		meshData->SetIndices(indices.data(), indexNum);
		meshData->SetSubMeshes(subMeshes.data(), subMeshes.size());

		return meshData;
	}
} // namesapce cube
