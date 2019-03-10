#include "Material.h"

#include "../../EngineCore.h"
#include "../RendererManager.h"
#include "Shader.h"
#include "MaterialInstance.h"

namespace cube
{
	HMaterial Material::Create(const MaterialInitializer& init)
	{
		auto& rendererManager = ECore().GetRendererManager();

		UPtr<Material> mat(new Material(init));
		mat->Initialize();

		return rendererManager.RegisterMaterial(std::move(mat));
	}

	Material::Material(const MaterialInitializer& init) : 
		mMaterialInit(init)
	{
	} 

	Material::~Material()
	{
	}

	SPtr<rt::RenderObject> Material::CreateRenderObject() const
	{
		SPtr<rt::Material> mat_rt(new rt::Material());

		RenderingThread::QueueSyncTask([this]() {
			GetRenderObject()->SyncMaterial(mMaterialInit);
		});

		return mat_rt;
	}

	HMaterialInstance Material::CreateInstance()
	{
		return ECore().GetRendererManager()._registerRenderObject(MaterialInstance::Create(GetHandler()));
	}

	void Material::Destroy()
	{
		UPtr<Material> ptr = ECore().GetRendererManager().UnregisterMaterial(GetHandler());

		RenderObject::Destroy();
	}

	namespace rt
	{
		Material::Material()
		{
		}

		void Material::Initialize()
		{
			SPtr<render::Device> device = ECore().GetRendererManager().GetDevice();

			using namespace render;

			mParamInfos = mMaterialInit.parameters;
			mShaders = mMaterialInit.shaders;

			ShaderParametersLayoutAttribute attr;
			attr.paramInfos.resize(mParamInfos.size());
			for(Uint64 i = 0; i < mParamInfos.size(); i++) {
				switch(mParamInfos[i].type) {
				case MaterialParameterType::Data:
					attr.paramInfos[i].type = ShaderParameterType::ConstantBuffer;
					break;

				case MaterialParameterType::Texture:
					attr.paramInfos[i].type = ShaderParameterType::SampledImage;
					break;

				default:
					ASSERTION_FAILED("Unknown MaterialParameterType ({0).", (int)mParamInfos[i].type);
					break;
				}
				attr.paramInfos[i].size = SCast(Uint32)(mParamInfos[i].dataSize);
				attr.paramInfos[i].count = 1;
				attr.paramInfos[i].bindIndex = SCast(Uint32)(i);
				attr.paramInfos[i].isChangedPerFrame = false;
				attr.paramInfos[i].debugName = "Material shader parameter layout param info";
			}
			attr.debugName = "Material shader parameter layout";
			mShaderParamsLayout = device->CreateShaderParametersLayout(attr);
		}

		void Material::Destroy()
		{
			mParamInfos.clear();
			mShaders.clear();
			mShaderParamsLayout = nullptr;
		}

		void Material::SyncMaterial(const MaterialInitializer& init)
		{
			mMaterialInit = init;
		}
	} // namespace rt
} // namespace cube
