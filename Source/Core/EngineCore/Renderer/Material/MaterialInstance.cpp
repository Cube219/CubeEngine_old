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
		const Vector<MaterialParameterInfo> paramInfos = mat->GetParameterInfos();

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

	UPtr<MaterialInstance> MaterialInstance::Create(HMaterial mat)
	{
		UPtr<MaterialInstance> matIns(new MaterialInstance(mat));
		matIns->Initialize();

		return matIns;
	}

	SPtr<rt::RenderObject> MaterialInstance::CreateRenderObject() const
	{
		SPtr<rt::MaterialInstance> matIns_rt(new rt::MaterialInstance(mMaterial->GetRenderObject()));
		
		RenderingThread::QueueSyncTask([this]() {
			GetRenderObject()->SyncMaterial(mMaterial->GetRenderObject());
		});

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
		UPtr<MaterialInstance> ptr = ECore().GetRendererManager()._unregisterRenderObject(GetHandler());

		RenderObject::Destroy();
	}

	namespace rt
	{
		MaterialInstance::MaterialInstance(SPtr<Material>& mat) : 
			mMaterial(mat)
		{
		}

		MaterialInstance::~MaterialInstance()
		{
		}

		void MaterialInstance::Initialize()
		{
			mShaderParameters = mMaterial->GetShaderParametersLayout()->CreateParameters();
		}

		void MaterialInstance::Destroy()
		{
			mShaderParameters = nullptr;
			mMaterial = nullptr;
		}

		void MaterialInstance::SyncMaterial(SPtr<rt::Material>& mat)
		{
			mMaterial = mat;
		}

		void MaterialInstance::SyncParameterData(Uint64 index, MaterialParameter& param)
		{
			MaterialParameter p;
			p.type = param.type;

			switch(param.type) {
			case MaterialParameterType::Data:
				p.size = param.size;
				p.data = (char*)malloc(p.size);
				memcpy(p.data, param.data, p.size);
				break;

			case MaterialParameterType::Texture:
				p.texture = param.texture;
				break;
			}

			mTempParameters.push_back(p);
			mTempParametersIndex.push_back(SCast(Uint32)(index));

			RenderingThread::QueueTask([this]() {
				ApplyParameters();
			});
		}

		void MaterialInstance::ApplyParameters()
		{
			for(Uint64 i = 0; i < mTempParameters.size(); i++) {
				auto& param = mTempParameters[i];

				switch(param.type) {
					case MaterialParameterType::Data:
						mShaderParameters->UpdateParameter(mTempParametersIndex[i], param.data, SCast(Uint32)(param.size));
						free(param.data);
						break;

					case MaterialParameterType::Texture:
						auto texture_rt = param.texture->GetRenderObject();

						mShaderParameters->UpdateParameter(mTempParametersIndex[i],
							texture_rt->GetTextureView(), texture_rt->GetSampler());
						break;
				}
			}

			mTempParameters.clear();
			mTempParametersIndex.clear();
		}
	} // namespace rt
} // namespace cube
