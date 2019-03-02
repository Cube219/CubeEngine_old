#include "MeshData.h"

#include "../Assertion.h"

namespace cube
{
	MeshData::MeshData(Uint64 vertexCount, Uint64 indexCount) : 
		mVertexCount(vertexCount),
		mIndexCount(indexCount),
		mTotalSize(vertexCount * sizeof(Vertex) + indexCount * sizeof(Index))
	{
		mData = malloc(mTotalSize);
	}

	MeshData::~MeshData()
	{
		free(mData);
	}

	void MeshData::SetVertices(Vertex* pVertices, Uint64 size)
	{
		CHECK(mVertexCount == size, "Size doen't match. (Expected: {0}, Actual: {1})", mVertexCount, size);

		memcpy(mData, pVertices, mVertexCount * sizeof(Vertex));
	}

	void MeshData::SetIndices(Index* pIndices, Uint64 size)
	{
		CHECK(mIndexCount == size, "Size doen't match. (Expected: {0}, Actual: {1})", mIndexCount, size);

		Uint8* dst = (Uint8*)mData + (mVertexCount * sizeof(Vertex));
		memcpy(dst, pIndices, mIndexCount * sizeof(Index));
	}

	void MeshData::SetSubMeshes(SubMesh* pSubMeshes, Uint64 size)
	{
		mSubMeshes.resize(size);
		for(Uint64 i = 0; i < size; i++) {
			mSubMeshes[i] = *pSubMeshes;
			pSubMeshes++;
		}
	}
} // namespace cube
