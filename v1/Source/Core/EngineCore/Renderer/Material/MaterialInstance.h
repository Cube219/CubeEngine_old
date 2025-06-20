#pragma once

#include "../../EngineCoreHeader.h"

#include "../RenderObject.h"
#include "../../Handler.h"
#include "RenderAPI/Interface/ShaderParameters.h"

namespace cube
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
		static UPtr<MaterialInstance> Create(HMaterial mat);

	public:
		~MaterialInstance();

		virtual SPtr<rt::RenderObject> CreateRenderObject() const override;
		SPtr<rt::MaterialInstance> GetRenderObject() const { return DPCast(rt::MaterialInstance)(mRenderObject); }

		HMaterialInstance GetHandler() const { return mMyHandler; }

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

		void SetParamData(StringRef name, void* pData, Uint64 dataSize);

		HashMap<String, Uint64> mParameterIndexLookupMap;
		Vector<MaterialParameter> mParameters;

		HMaterial mMaterial;
	};

	namespace rt
	{
		class MaterialInstance : public RenderObject
		{
		public:
			virtual ~MaterialInstance();
			virtual void Initialize() override;
			virtual void Destroy() override;

			void SyncMaterial(SPtr<rt::Material>& mat);
			void SyncParameterData(Uint64 index, MaterialParameter& param);

			SPtr<rt::Material> GetMaterial() const { return mMaterial; }

			SPtr<render::ShaderParameters> GetShaderParameters() const { return mShaderParameters; }

		private:
			friend class cube::MaterialInstance;
			friend class cube::Material;

			MaterialInstance(SPtr<Material>& mat);

			void ApplyParameters();

			SPtr<Material> mMaterial;

			Vector<Uint32> mTempParametersIndex;
			Vector<MaterialParameter> mTempParameters;
			SPtr<render::ShaderParameters> mShaderParameters;
		};
	} // namespace rt
} // namespace cube
