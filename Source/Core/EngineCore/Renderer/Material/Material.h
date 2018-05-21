#pragma once

#include "../../EngineCoreHeader.h"

#include "../RenderObject.h"
#include "BaseRenderAPI/Wrapper/Descriptor.h"
#include "../../BasicHandler.h"

namespace cube
{
	namespace core
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

			virtual SPtr<RenderObject_RT> CreateRenderObject_RT() const override;
			SPtr<Material_RT> GetRenderObject_RT() const { return DPCast(Material_RT)(mRenderObject_RT); }

			HMaterialInstance CreateInstance();

			void Destroy();

		private:
			friend class RendererManager;
			friend class MaterialInstance;

			Material(const MaterialInitializer& init);

			MaterialInitializer mMaterialInit;

			HMaterial mMyHandler;
		};

		class Material_RT : public RenderObject_RT
		{
		public:
			virtual ~Material_RT(){ }

			const Vector<MaterialParameterInfo>& GetParameterInfos() const { return mParamInfos; }

			const Vector<RPtr<Shader>>& GetShaders() const { return mShaders; }

			SPtr<render::DescriptorSetLayout> GetDescriptorSetLayout() const { return mDescriptorSetLayout; }

		private:
			friend class Material;
			friend class RendererManager;

			Material_RT(const MaterialInitializer& init);

			int mIndex = -1; // Used in RendererManager

			Vector<MaterialParameterInfo> mParamInfos;

			Vector<RPtr<Shader>> mShaders;
			SPtr<render::DescriptorSetLayout> mDescriptorSetLayout;

			SPtr<render::RenderAPI> mRenderAPI_ref;
		};

	} // namespace core
} // namespace cube
