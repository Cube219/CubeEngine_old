#pragma once

#include "../../EngineCoreHeader.h"

#include "../RenderObject.h"
#include "BaseRenderAPI/Wrapper/Buffer.h"
#include "BaseRenderAPI/Wrapper/Descriptor.h"
#include "../../BasicHandler.h"

namespace cube
{
	namespace core
	{
		struct MaterialParameter
		{
			MaterialParameterType type;
			char* data;
			size_t size;
			RPtr<Texture> texture; // Only use when the type is Texture
		};

		class ENGINE_CORE_EXPORT MaterialInstance : public RenderObject
		{
		public:
			static SPtr<MaterialInstance> Create(HMaterial mat);

		public:
			~MaterialInstance();

			virtual SPtr<RenderObject_RT> CreateRenderObject_RT() const override;
			SPtr<MaterialInstance_RT> GetRenderObject_RT() const { return DPCast(MaterialInstance_RT)(mRenderObject_RT); }

			HMaterial GetMaterial() const { return mMaterial; }

			template <typename T>
			void SetParameterData(String& name, T& data)
			{
				T& t = data;
				SetParamData(name, &data, sizeof(T));
			}

			template <>
			void SetParameterData(String& name, RPtr<Texture>& texture);

			void Destroy();

		private:
			friend class Material;
			MaterialInstance(HMaterial mat);

			void SetParamData(String& name, void* pData, uint64_t dataSize);

			HMaterialInstance mMyHandler;

			HashMap<String, uint64_t> mParameterIndexLookupMap;
			Vector<MaterialParameter> mParameters;

			HMaterial mMaterial;
		};

		class MaterialInstance_RT : public RenderObject_RT
		{
		public:
			virtual ~MaterialInstance_RT();

			void SyncParameterData(uint64_t index, MaterialParameter& param);

			SPtr<Material_RT> GetMaterial() const { return mMaterial; }

			SPtr<render::DescriptorSet> GetDescriptorSet() const { return mDescriptorSet; }

		private:
			friend class MaterialInstance;
			friend class Material;

			MaterialInstance_RT(SPtr<Material_RT>& mat);

			void WriteParameterInBuffer(uint64_t index);
			void WriteTextureParameterInBuffer(uint64_t index);

			SPtr<Material_RT> mMaterial;

			Vector<MaterialParameter> mParameters;
			SPtr<render::Buffer> mParametersBuffer;

			SPtr<render::DescriptorSet> mDescriptorSet;
		};
	} // namespace core
} // namespace cube
