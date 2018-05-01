#include "MaterialInstance.h"

#include "Material.h"
#include "../Texture.h"
#include "../../LogWriter.h"

namespace cube
{
	namespace core
	{
		MaterialInstance::MaterialInstance(SPtr<render::RenderAPI>& renderAPI, HMaterial mat) :
			mMaterial(mat)
		{
			mDescriptorSet = renderAPI->CreateDescriptorSet(mat->GetDescriptorSetLayout());

			// Create dataBuffer used in descriptor set based on parameterInfos
			render::BufferInitializer bufInit;
			bufInit.type = render::BufferTypeBits::Uniform;

			render::BufferInitializer::BufferData bufData;
			bufData.data = nullptr;

			Vector<MaterialParameterInfo>& paramInfos = mat->mParamInfos;
			uint64_t paramNum = paramInfos.size();
			for(uint64_t i = 0; i < paramNum; i++) {
				// Texture data is not saved in data buffer
				if(paramInfos[i].type != MaterialParameterType::Texture) {
					bufData.size = paramInfos[i].dataSize;
					bufInit.bufferDatas.push_back(bufData);
				}

				mParameterIndexLookupMap[paramInfos[i].name] = i;
			}
			if(bufInit.bufferDatas.size() > 0)
				mParametersBuffer = renderAPI->CreateBuffer(bufInit);

#ifdef _DEBUG
			mParamInfos = paramInfos;
#endif // _DEBUG
		}

		MaterialInstance::~MaterialInstance()
		{
		}

		void MaterialInstance::SetParamData(String& name, void* pData, uint64_t dataSize)
		{
			auto res = mParameterIndexLookupMap.find(name);
			if(res == mParameterIndexLookupMap.end()) {
				CUBE_LOG(LogType::Error, "Cannot find parameter name {0}.", name);
				return;
			}

			uint64_t paramIndex = res->second;

#ifdef _DEBUG
			if(dataSize != mParamInfos[paramIndex].dataSize) {
				CUBE_LOG(LogType::Error, "Wrong parameter size({0} != {1}).", mParamInfos[paramIndex].dataSize, dataSize);
				return;
			}
#endif // _DEBUG

			mParametersBuffer->Map();
			mParametersBuffer->UpdateBufferData(paramIndex, pData, dataSize);
			mParametersBuffer->Unmap();

			render::BufferInfo bufInfo = mParametersBuffer->GetInfo(paramIndex);
			mDescriptorSet->WriteBufferInDescriptor(paramIndex, 1, &bufInfo);
		}

		template<>
		void MaterialInstance::SetParameterData(String& name, RPtr<Texture>& texture)
		{
			auto res = mParameterIndexLookupMap.find(name);
			if(res == mParameterIndexLookupMap.end()) {
				CUBE_LOG(LogType::Error, "Cannot find parameter name {0}.", name);
				return;
			}

			uint64_t paramIndex = res->second;

#ifdef _DEBUG
			if(mParamInfos[paramIndex].type != MaterialParameterType::Texture) {
				CUBE_LOG(LogType::Error, "The Parameter {0} is not a Texture parameter.", name);
				return;
			}
#endif // _DEBUG

			SPtr<render::ImageView> imageView = texture->GetImageView();
			SPtr<render::Sampler> sampler = texture->GetSampler();

			mDescriptorSet->WriteImagesInDescriptor(paramIndex, 1, &imageView, &sampler);
		}

		void MaterialInstance::Destroy()
		{
			mMyHandler.mData->data = nullptr;
		}
	} // namespace core
} // namespace cube
