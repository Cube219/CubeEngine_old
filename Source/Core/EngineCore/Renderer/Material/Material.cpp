#include "Material.h"

#include "../Texture.h"
#include "../../LogWriter.h"
#include "Base/format.h"

namespace cube
{
	namespace core
	{
		Material::Material(const Vector<MaterialParameterInfo>& parameters) : 
			mIsUpdated(nullptr)
		{
			mParametersDataSize = 0;

			mParameters.resize(parameters.size());

			for(size_t i = 0; i < parameters.size(); i++) {
				auto paramInfo = parameters[i];

				MaterialParameter param;
				param.type = paramInfo.type;
				param.size = paramInfo.dataSize;

				if(paramInfo.type == MaterialParameterType::Data)
					mParametersDataSize += paramInfo.dataSize;
				else if(paramInfo.type == MaterialParameterType::Texture)
					param.size = 0; // Texture data isn't saved in mParametersData

				mParameters[i] = param;

				mParameterIndexLookupMap[paramInfo.name] = i;
			}

			// Allocate memory for parameters
			if(mParametersDataSize > 0){
				mParametersData = (char*)malloc(mParametersDataSize);
				char* currentDataPtr = mParametersData;

				for(size_t i = 0; i < mParameters.size(); i++) {
					mParameters[i].data = currentDataPtr;
					currentDataPtr += mParameters[i].size;
				}
			}
		}

		Material::~Material()
		{
			if(mParametersDataSize > 0)
				free(mParametersData);
		}

		template<typename T>
		void Material::SetParameterData(String& name, T& data)
		{
			auto res = mParameterIndexLookupMap.find(name);
			if(res == mParameterIndexLookupMap.end()) {
				//LogWriter::WriteLog(fmt::format(L"Material: Cannot find parameter name {0}.", name));
				return;
			}

			MaterialParameter param = mParameters[res->second];

			uint64_t dataSize = sizeof(data);
#ifdef _DEBUG
			if(dataSize != param.size) {
				//LogWriter::WriteLog(fmt::format(L"Material: Wrong parameter size({0} != {1}).", param.size, dataSize));
				return;
			}
#endif // _DEBUG

			memcpy(param.data, &data, dataSize);

			if(mIsUpdated != nullptr)
				*mIsUpdated = true;
		}

		template<>
		void Material::SetParameterData(String& name, SPtr<Texture>& texture)
		{
			auto res = mParameterIndexLookupMap.find(name);
			if(res == mParameterIndexLookupMap.end()) {
				//LogWriter::WriteLog(fmt::format(L"Material: Cannot find parameter name {0}.", name));
				return;
			}

			MaterialParameter& param = mParameters[res->second];

#ifdef _DEBUG
			if(param.type != MaterialParameterType::Texture) {
				//LogWriter::WriteLog(fmt::format(L"Material: The Parameter {0} is not a Texture parameter.", name));
				return;
			}
#endif // _DEBUG

			param.texture = texture;

			if(mIsUpdated != nullptr)
				*mIsUpdated = true;
		}
	}
}