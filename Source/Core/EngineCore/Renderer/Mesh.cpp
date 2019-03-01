#include "Mesh.h"

#include "../EngineCore.h"
#include "../Resource/ResourceManager.h"
#include "RendererManager.h"
#include "MeshData.h"

namespace cube
{
	Mesh::Mesh() : 
		mMeshData(nullptr)
	{
	}

	Mesh::Mesh(const SPtr<MeshData>& initialiData) :
		mMeshData(initialiData),
		mSubMeshes(initialiData->GetSubMeshes())
	{
	}

	Mesh::~Mesh()
	{
	}

	RPtr<Mesh> Mesh::Load(StringRef path)
	{
		auto mesh = ECore().GetResourceManager().LoadResource<Mesh>(path);
		mesh->Initialize();

		return mesh;
	}

	SPtr<rt::RenderObject> Mesh::CreateRenderObject() const
	{
		SPtr<rt::Mesh> mesh_rt(new rt::Mesh(mMeshData));

		if(mMeshData != nullptr) {
			RenderingThread::QueueSyncTask([meshData = mMeshData, mesh_rt]() {
				mesh_rt->SyncMesh(meshData);
			});
		}

		mMeshData = nullptr;

		return mesh_rt;
	}

	namespace rt
	{
		Mesh::Mesh(const SPtr<MeshData>& initialiData) :
			mMeshData(initialiData)
		{
		}

		Mesh::~Mesh()
		{
		}

		void Mesh::Initialize()
		{
		}

		void Mesh::SyncMesh(const SPtr<MeshData>& meshData)
		{
			mMeshData = meshData;

			FlushMeshToMeshBuffer();
		}

		void Mesh::FlushMeshToMeshBuffer()
		{
			mVertexOffset = 0;
			mIndexOffset = mMeshData->GetVertexCount() * sizeof(Vertex);
			mSubMeshes = mMeshData->GetSubMeshes();

			using namespace render;

			BufferAttribute attr;
			attr.pData = mMeshData->GetData();
			attr.size = mMeshData->GetTotalSize();
			attr.cpuAccessible = false;
			attr.usage = ResourceUsage::Default;
			attr.bindTypeFlags = BufferBindTypeFlagBits::Vertex_Bit | BufferBindTypeFlagBits::Index_Bit;
			attr.debugName = "Mesh";

			mMeshBuffer = ECore().GetRendererManager().GetDevice()->CreateBuffer(attr);

			mMeshData = nullptr;
		}
	} // namespace rt
} // namespace cube
