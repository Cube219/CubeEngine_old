#include "Renderer3D.h"

#include "../EngineCore.h"
#include "RendererManager.h"
#include "Base/MatrixUtility.h"
#include "Mesh.h"
#include "Material/Material.h"
#include "Material/MaterialInstance.h"
#include "Texture.h"
#include "Material/Shader.h"
#include "CameraRenderer3D.h"
#include "RenderAPI/Interface/Device.h"

namespace cube
{
	namespace core
	{
		Renderer3D::Renderer3D()
		{
		}

		Renderer3D::~Renderer3D()
		{
		}

		SPtr<Renderer3D> Renderer3D::Create()
		{
			SPtr<Renderer3D> renderer3d(new Renderer3D());
			renderer3d->Initialize();

			return renderer3d;
		}

		SPtr<rt::RenderObject> Renderer3D::CreateRenderObject() const
		{
			SPtr<rt::Renderer3D> renderer3d_rt(new rt::Renderer3D());
			renderer3d_rt->Initialize();

			return renderer3d_rt;
		}

		void Renderer3D::SetMesh(RPtr<Mesh>& mesh)
		{
			mMesh = mesh;
			mMaterialInses.resize(mMesh->GetSubMeshes().size());
			QueueSyncTask([this]() {
				GetRenderObject()->SyncMesh(mMesh);
			});
		}

		void Renderer3D::SetMaterialInstance(HMaterialInstance& materialIns, uint32_t index)
		{
			mMaterialInses[index] = materialIns;
			QueueSyncTask([this, materialIns, index]() {
				GetRenderObject()->SyncMaterialInstance(materialIns, index);
			});
		}

		void Renderer3D::SetModelMatrix(const Matrix& modelMatrix)
		{
			mModelMatrix = modelMatrix;
			QueueSyncTask([this]() {
				GetRenderObject()->SyncModelMatrix(mModelMatrix);
			});
		}

		namespace rt
		{
			Renderer3D::Renderer3D()
			{
				RendererManager& rm = ECore().GetRendererManager();
				mDevice = rm.GetDevice();

				SPtr<render::ShaderParametersLayout> perObjectShaderParametersLayout = rm._GetPerObjectShaderParametersLayout();
				mShaderParameters = perObjectShaderParametersLayout->CreateParameters();
			}

			void Renderer3D::SyncMesh(RPtr<Mesh>& mesh)
			{
				mMesh = mesh;
				mMaterialInses.resize(mMesh->GetSubMeshes().size());
				mIsMeshUpdated = true;
			}

			void Renderer3D::SyncMaterialInstance(HMaterialInstance materialIns, uint32_t index)
			{
				mMaterialInses[index] = materialIns->GetRenderObject();
			}

			void Renderer3D::SyncModelMatrix(const Matrix& modelMatrix)
			{
				mUBOPerObject.modelMatrix = modelMatrix;
			}

			void Renderer3D::PrepareDraw(SPtr<render::CommandList>& commandList, SPtr<rt::CameraRenderer3D>& camera)
			{
				if(mIsMeshUpdated == true) {
					Vector<Vertex>& vertices = mMesh->GetVertex();
					Vector<Index>& indices = mMesh->GetIndex();

					RecreateDataBuffer();

					memcpy((Uint8*)mDataBufferMappedPtr + mVertexOffset, vertices.data(), vertices.size() * sizeof(Vertex));
					memcpy((Uint8*)mDataBufferMappedPtr + mIndexOffset, indices.data(), indices.size() * sizeof(Index));

					mDataBuffer->Unmap();

					mIsMeshUpdated = false;
				}

				// Update mvp matrix
				mUBOPerObject.mvp = mUBOPerObject.modelMatrix * camera->GetViewProjectionMatrix();

				mShaderParameters->UpdateParameter(0, &mUBOPerObject, sizeof(UBOPerObject));

				// Bind vertex / index data
				commandList->BindVertexBuffers(0, 1, &mDataBuffer, &mVertexOffset);
				commandList->BindIndexBuffer(mDataBuffer, mIndexOffset);
			}

			void Renderer3D::RecreateDataBuffer()
			{
				using namespace render;

				BufferAttribute attr;
				attr.size = mMesh->GetVertex().size() * sizeof(Vertex) + mMesh->GetIndex().size() * sizeof(Index);
				attr.cpuAccessible = true;
				attr.usage = ResourceUsage::Dynamic;
				attr.bindTypeFlags = BufferBindTypeFlagBits::Uniform_Bit | BufferBindTypeFlagBits::Vertex_Bit | BufferBindTypeFlagBits::Index_Bit;
				attr.pData = nullptr;
				attr.isDedicated = false;
				attr.debugName = "Renderer3D data buffer";

				mDataBuffer = mDevice->CreateBuffer(attr);
				mDataBuffer->Map(mDataBufferMappedPtr);

				mVertexOffset = 0;
				mIndexOffset = mMesh->GetVertex().size() * sizeof(Vertex);
			}
		} // namespace rt
	} // namespace core
} // namespace cube
