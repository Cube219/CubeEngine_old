#include "Mesh.h"

#include "../EngineCore.h"
#include "../Resource/ResourceManager.h"
#include "RendererManager.h"
#include "MeshData.h"

namespace cube
{
	RPtr<Mesh> Mesh::Load(StringRef path)
	{
		auto mesh = ECore().GetResourceManager().LoadResource<Mesh>(path);
		mesh->Initialize();

		return mesh;
	}

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

	SPtr<rt::RenderObject> Mesh::CreateRenderObject() const
	{
		SPtr<rt::Mesh> mesh_rt(new rt::Mesh(mMeshData));

		mMeshData = nullptr;

		return mesh_rt;
	}

	namespace rt
	{
		Mesh::Mesh(const SPtr<MeshData>& initialiData) :
			mMeshData(initialiData),
			mVertexOffset(0),
			mIndexOffset(mMeshData->GetVertexCount() * sizeof(Vertex)),
			mSubMeshes(mMeshData->GetSubMeshes())
		{
		}

		Mesh::~Mesh()
		{
		}

		void Mesh::Initialize()
		{
			if(mMeshData != nullptr) {
				FlushToMeshBuffer();
			}
		}

		void Mesh::Destroy()
		{
		}

		void Mesh::SyncMeshData(const SPtr<MeshData>& meshData)
		{
			mMeshData = meshData;

			RenderingThread::QueueTask([this]() {
				FlushToMeshBuffer();
			});
		}

		void Mesh::FlushToMeshBuffer()
		{
			using namespace render;

			BufferAttribute attr;
			attr.pData = mMeshData->GetData();
			attr.size = mMeshData->GetTotalSize();
			attr.cpuAccessible = false;
			attr.usage = ResourceUsage::Default;
			attr.bindTypeFlags = BufferBindTypeFlag::Vertex | BufferBindTypeFlag::Index;
			attr.debugName = "Mesh";

			mMeshBuffer = ECore().GetRendererManager().GetDevice()->CreateBuffer(attr);

			mMeshData = nullptr;
		}
	} // namespace rt
} // namespace cube
