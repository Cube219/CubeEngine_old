#pragma once

#include "../RenderAPIHeader.h"
#include "EngineCore/Assertion.h"

#include "../Utilities/BaseRenderObject.h"

namespace cube
{
	namespace render
	{
		struct ShaderParameterInfo
		{
			ShaderParameterType type;
			Uint32 size;
			Uint32 count;
			Uint32 bindIndex;
			bool isChangedPerFrame;

			const char* debugName = "";
		};

		// Helper structure to store parameter infos
		struct ShaderParameterInfoList
		{
			ShaderParameterInfoList(const Vector<ShaderParameterInfo>& infos)
			{
				parameterInfos = infos;

				Uint32 maxBindIndex = 0;
				for(auto& info : parameterInfos) {
					if(maxBindIndex < info.bindIndex)
						maxBindIndex = info.bindIndex;
				}
				parameterInfoBindIndexLookupTable.resize(maxBindIndex + 1, Uint32InvalidValue);
				for(Uint64 i = 0; i < parameterInfos.size(); i++) {
					parameterInfoBindIndexLookupTable[parameterInfos[i].bindIndex] = SCast(Uint32)(i);
				}
			}

			ShaderParameterInfo& operator[](int index) { return parameterInfos[index]; }

			ShaderParameterInfo& FindInfo(Uint32 bindIndex)
			{
				CHECK(bindIndex < parameterInfoBindIndexLookupTable.size(), "Cannot find shader parameter whose bind index is '{0}'.", bindIndex);

				Uint32 res = parameterInfoBindIndexLookupTable[bindIndex];

				CHECK(res != Uint32InvalidValue, "Cannot find shader parameter whose bind index is '{0}'.", bindIndex);

				return parameterInfos[res];
			}

			Vector<ShaderParameterInfo> parameterInfos;
			Vector<Uint32> parameterInfoBindIndexLookupTable;
		};

		struct ShaderParametersLayoutAttribute : public BaseAttribute
		{
			Vector<ShaderParameterInfo> paramInfos;
		};

		class ShaderParametersLayout : public BaseRenderObject
		{
		public:
			ShaderParametersLayout(const ShaderParametersLayoutAttribute& attr) :
				BaseRenderObject(attr.debugName)
			{}
			virtual ~ShaderParametersLayout() {}

			virtual SPtr<ShaderParameters> CreateParameters() = 0;
		};
	} // namespace render
} // namespace cube
