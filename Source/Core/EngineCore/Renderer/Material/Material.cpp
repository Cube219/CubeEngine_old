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
			mRenderAPI_ref = ECore().GetRendererManager().GetRenderAPI();

			using namespace render;

			mParamInfos = init.parameters;
			mShaders = init.shaders;

			render::DescriptorSetInitializer descSetInit;
			render::DescriptorSetInitializer::Descriptor desc;
			desc.shaderType = ShaderTypeBits::Vertex | ShaderTypeBits::Fragment; // TODO: 사용자 설정으로
			desc.count = 1;
			for(uint32_t i = 0; i < mParamInfos.size(); i++) {
				desc.bindingIndex = i;

				switch(mParamInfos[i].type) {
					case MaterialParameterType::Data:
						desc.type = DescriptorType::UniformBuffer;
						break;

					case MaterialParameterType::Texture:
						desc.type = DescriptorType::CombinedImageSampler;
						break;

					default:
						break;
				}
				descSetInit.descriptors.push_back(desc);
			}
			mDescriptorSetLayout = mRenderAPI_ref->CreateDescriptorSetLayout(descSetInit);
		}
	} // namespace core
} // namespace cube
