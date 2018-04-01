#include "Material.h"

#include "../../EngineCore.h"
#include "../RendererManager.h"
#include "Shader.h"
#include "MaterialInstance.h"

namespace cube
{
	namespace core
	{
		SPtr<Material> Material::Create(const MaterialInitializer& init)
		{
			auto rendererManager = ECore()->GetRendererManager();

			SPtr<Material> mat = std::make_shared<Material>(rendererManager->GetRenderAPI(), init);
			rendererManager->RegisterMaterial(mat);

			return mat;
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

		SPtr<MaterialInstance> Material::CreateInstance()
		{
			SPtr<MaterialInstance> ins(new MaterialInstance(mRenderAPI_ref, shared_from_this()));
			return ins;
		}
	} // namespace core
} // namespace cube
