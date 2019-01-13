#include "MaterialInstance.h"

#include "../../EngineCore.h"
#include "../RendererManager.h"
#include "BaseRenderAPI_New/Interface/Device.h"
#include "Material.h"
#include "../Texture.h"
#include "../../Assertion.h"

namespace cube
{
	namespace core
	{
		MaterialInstance::MaterialInstance(HMaterial mat) :
			mMaterial(mat)
		{
			const Vector<MaterialParameterInfo> paramInfos = mat->GetRenderObject_RT()->GetParameterInfos();

			uint64_t paramNum = paramInfos.size();
			mParameters.resize(paramNum);

			for(uint64_t i = 0; i < paramNum; i++) {
				mParameterIndexLookupMap[paramInfos[i].name] = i;

				mParameters[i].type = paramInfos[i].type;
				mParameters[i].size = paramInfos[i].dataSize;
				if(mParameters[i].type == MaterialParameterType::Data) {
					mParameters[i].data = (char*)malloc(paramInfos[i].dataSize);
				}
			}
		}

		MaterialInstance::~MaterialInstance()
		{
			for(auto& param : mParameters) {
				if(param.type == MaterialParameterType::Data) {
					free(param.data);
				}
			}
		}

		SPtr<MaterialInstance> MaterialInstance::Create(HMaterial mat)
		{
			SPtr<MaterialInstance> matIns(new MaterialInstance(mat));
			matIns->Initialize();

			return matIns;
		}

		SPtr<RenderObject_RT> MaterialInstance::CreateRenderObject_RT() const
		{
			SPtr<MaterialInstance_RT> matIns_rt(new MaterialInstance_RT(mMaterial->GetRenderObject_RT()));
			matIns_rt->Initialize();

			return matIns_rt;
		}

		void MaterialInstance::SetParamData(StringRef name, void* pData, uint64_t dataSize)
		{
			auto res = mParameterIndexLookupMap.find(name.GetString());
			CHECK(res != mParameterIndexLookupMap.end(), "Cannot find parameter name '{0}'.", name.GetString());

			uint64_t paramIndex = res->second;

			if(dataSize != mParameters[paramIndex].size) {
				CUBE_LOG(LogType::Warning, "Wrong parameter size. (Expected: {0} / Actual: {1})",
					mParameters[paramIndex].size, dataSize);
			}

			memcpy(mParameters[paramIndex].data, pData, dataSize);

			QueueSyncTask([this, paramIndex]() {
				GetRenderObject_RT()->SyncParameterData(paramIndex, mParameters[paramIndex]);
			});
		}

		template<>
		void MaterialInstance::SetParameterData(StringRef name, RPtr<Texture>& texture)
		{
			auto res = mParameterIndexLookupMap.find(name.GetString());
			CHECK(res != mParameterIndexLookupMap.end(), "Cannot find parameter name '{0}'.", name.GetString());

			uint64_t paramIndex = res->second;


			CHECK(mParameters[paramIndex].type == MaterialParameterType::Texture,
				"The Parameter '{0}' is not a Texture parameter.", name.GetString());

			mParameters[paramIndex].texture = texture;

			QueueSyncTask([this, paramIndex]() {
				GetRenderObject_RT()->SyncParameterData(paramIndex, mParameters[paramIndex]);
			});
		}

		void MaterialInstance::Destroy()
		{
			mMyHandler.mData->data = nullptr;
		}

		MaterialInstance_RT::MaterialInstance_RT(SPtr<Material_RT>& mat)
		{
			using namespace render;

			SPtr<Device> device = ECore().GetRendererManager().GetDevice();

			mShaderParameters = mat->GetShaderParametersLayout()->CreateParameters();

			const Vector<MaterialParameterInfo>& paramInfos = mat->GetParameterInfos();

			uint64_t paramNum = paramInfos.size();
			mParameters.resize(paramNum);

			/*
			for(uint64_t i = 0; i < paramNum; i++) {
				mParameters[i].type = paramInfos[i].type;
				mParameters[i].size = paramInfos[i].dataSize;

				// Texture data is not saved in data buffer
				if(paramInfos[i].type != MaterialParameterType::Texture) {
					bufData.size = paramInfos[i].dataSize;
					bufInit.bufferDatas.push_back(bufData);

					mParameters[i].data = (char*)malloc(paramInfos[i].dataSize);
				}
			}
			if(bufInit.bufferDatas.size() > 0)
				mParametersBuffer = renderAPI->CreateBuffer(bufInit);
				*/
		}

		MaterialInstance_RT::~MaterialInstance_RT()
		{
			/*
			for(auto& param : mParameters) {
				if(param.type == MaterialParameterType::Data) {
					free(param.data);
				}
			}
			*/
		}

		void MaterialInstance_RT::SyncParameterData(uint64_t index, MaterialParameter& param)
		{
			mParameters[index].type = param.type;
			mParameters[index].size = param.size;

			switch(param.type) {
				case MaterialParameterType::Data:
					mShaderParameters->UpdateParameter(index, mParameters[index].data, mParameters[index].size);
					break;

				case MaterialParameterType::Texture:
					mShaderParameters->UpdateParameter(index,
						mParameters[index].texture->GetTextureView(), mParameters[index].texture->GetSampler());
					break;
			}
		}
	} // namespace core
} // namespace cube
