#pragma once

#include "../../EngineCoreHeader.h"

#include "../RenderObject.h"
#include "../../BasicHandler.h"
#include "RenderAPI/Interface/ShaderParameters.h"

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

			virtual SPtr<rt::RenderObject> CreateRenderObject() const override;
			SPtr<rt::MaterialInstance> GetRenderObject() const { return DPCast(rt::MaterialInstance)(mRenderObject); }

			HMaterial GetMaterial() const { return mMaterial; }

			template <typename T>
			void SetParameterData(StringRef name, T& data)
			{
				T& t = data;
				SetParamData(name, &data, sizeof(T));
			}

			template <>
			void SetParameterData(StringRef name, RPtr<Texture>& texture);

			void Destroy();

		private:
			friend class Material;
			MaterialInstance(HMaterial mat);

			void SetParamData(StringRef name, void* pData, uint64_t dataSize);

			HMaterialInstance mMyHandler;

			HashMap<String, uint64_t> mParameterIndexLookupMap;
			Vector<MaterialParameter> mParameters;

			HMaterial mMaterial;
		};

		namespace rt
		{
			class MaterialInstance : public RenderObject
			{
			public:
				virtual ~MaterialInstance();

				void SyncParameterData(uint64_t index, MaterialParameter& param);

				SPtr<rt::Material> GetMaterial() const { return mMaterial; }

				SPtr<render::ShaderParameters> GetShaderParameters() const { return mShaderParameters; }

			private:
				friend class cube::core::MaterialInstance;
				friend class cube::core::Material;

				MaterialInstance(SPtr<rt::Material>& mat);

				SPtr<rt::Material> mMaterial;

				SPtr<render::ShaderParameters> mShaderParameters;
			};
		} // namespace rt
	} // namespace core
} // namespace cube
