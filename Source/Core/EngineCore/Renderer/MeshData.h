#pragma once

#include "../EngineCoreHeader.h"

#include "Vertex.h"
#include "Mesh.h"

namespace cube
{
	class ENGINE_CORE_EXPORT MeshData
	{
	public:
		MeshData(Uint64 vertexCount, Uint64 indexCount);
		~MeshData();

		MeshData(const MeshData& other) :
			mSubMeshes(other.mSubMeshes),
			mVertexCount(other.mVertexCount),
			mIndexCount(other.mIndexCount),
			mTotalSize(other.mTotalSize)
		{
			mData = malloc(mTotalSize);
			memcpy(mData, other.mData, mTotalSize);
		}

		MeshData& operator=(const MeshData& rhs)
		{
			if(this == &rhs)
				return *this;

			mSubMeshes = rhs.mSubMeshes;
			mVertexCount = rhs.mVertexCount;
			mIndexCount = rhs.mIndexCount;

			if(mTotalSize != rhs.mTotalSize) {
				free(mData);
				mData = malloc(rhs.mTotalSize);

				mTotalSize = rhs.mTotalSize;
			}

			memcpy(mData, rhs.mData, mTotalSize);

			return *this;
		}

		void SetVertices(Vertex* pVertices, Uint64 size);
		void SetIndices(Index* pIndices, Uint64 size);
		void SetSubMeshes(SubMesh* pSubMeshes, Uint64 size);

		Uint64 GetVertexCount() const { return mVertexCount; }
		Uint64 GetIndexCount() const { return mIndexCount; }
		const Vector<SubMesh>& GetSubMeshes() const { return mSubMeshes; }
		void* GetData() const { return mData; }
		Uint64 GetTotalSize() const { return mTotalSize; }

	private:
		Uint64 mVertexCount;
		Uint64 mIndexCount;
		Uint64 mTotalSize;

		void* mData; // Combine vertex/index
		Vector<SubMesh> mSubMeshes;
	};
} // namespace cube
