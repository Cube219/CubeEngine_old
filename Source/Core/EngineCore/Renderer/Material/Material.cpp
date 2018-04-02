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
			auto rendererManager = ECore()->GetRendererManager();

			SPtr<Material> mat(new Material(rendererManager->GetRenderAPI(), init));

			return rendererManager->RegisterMaterial(mat);
		}

		Material::Material(SPtr<BaseRenderAPI>& renderAPI, const MaterialInitializer& init) : 
			mRenderAPI_ref(renderAPI)
		{
			mParamInfos = init.parameters;
			mShaders = init.shaders;

			BaseRenderDescriptorSetInitializer descSetInit;
			BaseRenderDescriptorSetInitializer::Descriptor desc;
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
			mDescriptorSetLayout = renderAPI->CreateDescriptorSetLayout(descSetInit);
		} 

		Material::~Material()
		{
		}

		HMaterialInstance Material::CreateInstance()
		{
			SPtr<MaterialInstanceData> matInsDataPtr = std::make_shared<MaterialInstanceData>();
			matInsDataPtr->data = UPtr<MaterialInstance>(new MaterialInstance(mRenderAPI_ref, mMyHandler));
			matInsDataPtr->data->mMyHandler = HMaterialInstance(matInsDataPtr);

			return HMaterialInstance(matInsDataPtr);
		}

		void Material::Destroy()
		{
			ECore()->GetRendererManager()->UnregisterMaterial(mMyHandler);
		}
	} // namespace core
} // namespace cube
