#include "Material.h"

#include "../../EngineCore.h"
#include "../RendererManager.h"
#include "Shader.h"
#include "MaterialInstance.h"

namespace cube
{
	namespace core
	{
		HMaterial Material::Create(const MaterialInitializer& init)
		{
			auto& rendererManager = ECore().GetRendererManager();

			SPtr<Material> mat(new Material(init));
			mat->Initialize();

			return rendererManager.RegisterMaterial(mat);
		}

		Material::Material(const MaterialInitializer& init) : 
			mMaterialInit(init)
		{
		} 

		Material::~Material()
		{
		}

		SPtr<RenderObject_RT> Material::CreateRenderObject_RT() const
		{
			SPtr<Material_RT> mat_rt(new Material_RT(mMaterialInit));
			mat_rt->Initialize();

			return mat_rt;
		}

		HMaterialInstance Material::CreateInstance()
		{
			SPtr<MaterialInstanceData> matInsDataPtr = std::make_shared<MaterialInstanceData>();
			matInsDataPtr->data = MaterialInstance::Create(mMyHandler);
			matInsDataPtr->data->mMyHandler = HMaterialInstance(matInsDataPtr);

			matInsDataPtr->data->GetRenderObject_RT()->mMaterial = GetRenderObject_RT();

			return HMaterialInstance(matInsDataPtr);
		}

		void Material::Destroy()
		{
			ECore().GetRendererManager().UnregisterMaterial(mMyHandler);
		}

		Material_RT::Material_RT(const MaterialInitializer& init)
		{
			mDevice = ECore().GetRendererManager().GetDevice();

			using namespace render;

			mParamInfos = init.parameters;
			mShaders = init.shaders;

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
				attr.paramInfos[i].size = mParamInfos[i].dataSize;
				attr.paramInfos[i].count = 1;
				attr.paramInfos[i].bindIndex = i;
				attr.paramInfos[i].isChangedPerFrame = false;
				attr.paramInfos[i].debugName = "Material shader parameter layout param info";
			}
			attr.debugName = "Material shader parameter layout";
			mShaderParamsLayout = mDevice->CreateShaderParametersLayout(attr);
		}
	} // namespace core
} // namespace cube
