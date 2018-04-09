#pragma once

#include "../../EngineCoreHeader.h"

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

		class ENGINE_CORE_EXPORT Material
		{
		public:
			static HMaterial Create(const MaterialInitializer& init);

		public:
			~Material();

			HMaterialInstance CreateInstance();

			const Vector<RPtr<Shader>>& GetShaders() const { return mShaders; }

			SPtr<render::DescriptorSetLayout> GetDescriptorSetLayout() const { return mDescriptorSetLayout; }

			void Destroy();

		private:
			friend class RendererManager;
			friend class MaterialInstance;

			Material(SPtr<render::RenderAPI>& renderAPI, const MaterialInitializer& init);

			HMaterial mMyHandler;

			int mIndex = -1; // Used in RendererManager

			Vector<MaterialParameterInfo> mParamInfos;

			Vector<RPtr<Shader>> mShaders;
			SPtr<render::DescriptorSetLayout> mDescriptorSetLayout;

			SPtr<render::RenderAPI> mRenderAPI_ref;
		};
	} // namespace core
} // namespace cube
