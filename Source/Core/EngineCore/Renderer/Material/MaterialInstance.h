#pragma once

#include "../../EngineCoreHeader.h"

#include "BaseRenderAPI/RenderAPI.h"
#include "BaseRenderAPI/Wrapper/Buffer.h"
#include "BaseRenderAPI/Wrapper/Descriptor.h"
#include "../../BasicHandler.h"

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

			HMaterial GetMaterial() const { return mMaterial; }
			
			SPtr<render::DescriptorSet> GetDescriptorSet() const { return mDescriptorSet; }

			template <typename T>
			void SetParameterData(String& name, T& data);

			template <>
			void SetParameterData(String& name, RPtr<Texture>& texture);

			void Destroy();

		private:
			friend class Material;
			MaterialInstance(SPtr<render::RenderAPI>& renderAPI, HMaterial mat);

			HMaterialInstance mMyHandler;

			HashMap<String, uint64_t> mParameterIndexLookupMap;

			SPtr<render::Buffer> mParametersBuffer;
#ifdef _DEBUG
			Vector<MaterialParameterInfo> mParamInfos;
#endif // _DEBUG

			SPtr<render::DescriptorSet> mDescriptorSet;

			HMaterial mMaterial;
		};
	} // namespace core
} // namespace cube
