#pragma once

#include "../../EngineCoreHeader.h"

#include "BaseRenderAPI/BaseRenderAPI.h"
#include "BaseRenderAPI/Wrapper/BaseRenderBuffer.h"
#include "BaseRenderAPI/Wrapper/BaseRenderDescriptor.h"

namespace cube
{
	namespace core
	{
		class ENGINE_CORE_EXPORT MaterialInstance
		{
		public:
			struct MaterialParameter
			{
				MaterialParameterType type;
				char* data;
				size_t size;
				RPtr<Texture> texture; // Only use when the type is Texture
			};

			~MaterialInstance();

			SPtr<Material> GetMaterial() const { return mMaterialRef; }
			
			SPtr<BaseRenderDescriptorSet> GetDescriptorSet() const { return mDescriptorSet; }

			template <typename T>
			void SetParameterData(String& name, T& data);

			template <>
			void SetParameterData(String& name, RPtr<Texture>& texture);

		private:
			friend class Material;
			MaterialInstance(SPtr<BaseRenderAPI>& renderAPI, SPtr<Material> mat);

			HashMap<String, uint64_t> mParameterIndexLookupMap;

			SPtr<BaseRenderBuffer> mParametersBuffer;
#ifdef _DEBUG
			Vector<MaterialParameterInfo> mParamInfos;
#endif // _DEBUG

			SPtr<BaseRenderDescriptorSet> mDescriptorSet;

			SPtr<Material> mMaterialRef;
		};
	}
}