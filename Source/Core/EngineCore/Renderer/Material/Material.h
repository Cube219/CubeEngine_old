#pragma once

#include "../../EngineCoreHeader.h"

#include "../RenderObject.h"
#include "../../BasicHandler.h"
#include "RenderAPI/Interface/ShaderParametersLayout.h"

namespace cube
{
	enum class MaterialParameterType
	{
		Data,
		Texture
	};

	struct ENGINE_CORE_EXPORT MaterialParameterInfo
	{
		String name;
		MaterialParameterType type;
		size_t dataSize;
	};

	struct MaterialInitializer
	{
		Vector<RPtr<Shader>> shaders;
		Vector<MaterialParameterInfo> parameters;
	};

	class ENGINE_CORE_EXPORT Material : public RenderObject
	{
	public:
		static HMaterial Create(const MaterialInitializer& init);

	public:
		virtual ~Material();

		virtual SPtr<rt::RenderObject> CreateRenderObject() const override;
		SPtr<rt::Material> GetRenderObject() const { return DPCast(rt::Material)(mRenderObject); }

		HMaterialInstance CreateInstance();

		void Destroy();

	private:
		friend class RendererManager;
		friend class MaterialInstance;

		Material(const MaterialInitializer& init);

		MaterialInitializer mMaterialInit;

		HMaterial mMyHandler;
	};

	namespace rt
	{
		class Material : public RenderObject
		{
		public:
			virtual ~Material() {}
			virtual void Initialize() override;

			const Vector<MaterialParameterInfo>& GetParameterInfos() const { return mParamInfos; }

			const Vector<RPtr<Shader>>& GetShaders() const { return mShaders; }

			SPtr<render::ShaderParametersLayout> GetShaderParametersLayout() const { return mShaderParamsLayout; }

		private:
			friend class cube::Material;
			friend class RendererManager;

			Material(const MaterialInitializer& init);

			int mIndex = -1; // Used in RendererManager

			Vector<MaterialParameterInfo> mParamInfos;

			Vector<RPtr<Shader>> mShaders;
			SPtr<render::ShaderParametersLayout> mShaderParamsLayout;

			SPtr<render::Device> mDevice;
		};
	} // namespace rt
} // namespace cube
