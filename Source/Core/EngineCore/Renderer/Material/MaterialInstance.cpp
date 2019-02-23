#include "MaterialInstance.h"

#include "../../EngineCore.h"
#include "../RendererManager.h"
#include "RenderAPI/Interface/Device.h"
#include "Material.h"
#include "../Texture.h"
#include "../../Assertion.h"

namespace cube
{
	MaterialInstance::MaterialInstance(HMaterial mat) :
		mMaterial(mat)
	{
		const Vector<MaterialParameterInfo> paramInfos = mat->GetRenderObject()->GetParameterInfos();

		Uint64 paramNum = paramInfos.size();
		mParameters.resize(paramNum);

		for(Uint64 i = 0; i < paramNum; i++) {
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

	SPtr<rt::RenderObject> MaterialInstance::CreateRenderObject() const
	{
		SPtr<rt::MaterialInstance> matIns_rt(new rt::MaterialInstance(mMaterial->GetRenderObject()));
		matIns_rt->Initialize();

		return matIns_rt;
	}

	void MaterialInstance::SetParamData(StringRef name, void* pData, Uint64 dataSize)
	{
		auto res = mParameterIndexLookupMap.find(name.GetString());
		CHECK(res != mParameterIndexLookupMap.end(), "Cannot find parameter name '{0}'.", name.GetString());

		Uint64 paramIndex = res->second;

		if(dataSize != mParameters[paramIndex].size) {
			CUBE_LOG(LogType::Warning, "Wrong parameter size. (Expected: {0} / Actual: {1})",
				mParameters[paramIndex].size, dataSize);
		}

		memcpy(mParameters[paramIndex].data, pData, dataSize);

		QueueSyncTask([this, paramIndex]() {
			GetRenderObject()->SyncParameterData(paramIndex, mParameters[paramIndex]);
		});
	}

	template<>
	void MaterialInstance::SetParameterData(StringRef name, RPtr<Texture>& texture)
	{
		auto res = mParameterIndexLookupMap.find(name.GetString());
		CHECK(res != mParameterIndexLookupMap.end(), "Cannot find parameter name '{0}'.", name.GetString());

		Uint64 paramIndex = res->second;


		CHECK(mParameters[paramIndex].type == MaterialParameterType::Texture,
			"The Parameter '{0}' is not a Texture parameter.", name.GetString());

		mParameters[paramIndex].texture = texture;

		QueueSyncTask([this, paramIndex]() {
			GetRenderObject()->SyncParameterData(paramIndex, mParameters[paramIndex]);
		});
	}

	void MaterialInstance::Destroy()
	{
		mMyHandler.mData->data = nullptr;
	}

	namespace rt
	{
		MaterialInstance::MaterialInstance(SPtr<rt::Material>& mat)
		{
			using namespace render;

			SPtr<Device> device = ECore().GetRendererManager().GetDevice();

			mShaderParameters = mat->GetShaderParametersLayout()->CreateParameters();
		}

		MaterialInstance::~MaterialInstance()
		{
		}

		void MaterialInstance::SyncParameterData(Uint64 index, MaterialParameter& param)
		{
			switch(param.type) {
			case MaterialParameterType::Data:
				mShaderParameters->UpdateParameter(index, param.data, param.size);
				break;

			case MaterialParameterType::Texture:
				mShaderParameters->UpdateParameter(index,
					param.texture->GetTextureView(), param.texture->GetSampler());
				break;
			}
		}
	} // namespace rt
} // namespace cube
