#pragma once

#include "../../EngineCoreHeader.h"

#include "BaseRenderAPI/Wrapper/BaseRenderDescriptor.h"

#include <glm.hpp>

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

		class ENGINE_CORE_EXPORT Material : public std::enable_shared_from_this<Material>
		{
		public:
			static SPtr<Material> Create(const MaterialInitializer& init);

		public:
			Material(SPtr<BaseRenderAPI>& renderAPI, const MaterialInitializer& init);
			~Material();

			SPtr<MaterialInstance> CreateInstance();

			const Vector<RPtr<Shader>>& GetShaders() const { return mShaders; }

			SPtr<BaseRenderDescriptorSetLayout> GetDescriptorSetLayout() const { return mDescriptorSetLayout; }

		private:
			friend class RendererManager;
			friend class MaterialInstance;

			int mIndex = -1; // Used in RendererManager

			Vector<MaterialParameterInfo> mParamInfos;

			Vector<RPtr<Shader>> mShaders;
			SPtr<BaseRenderDescriptorSetLayout> mDescriptorSetLayout;

			SPtr<BaseRenderAPI> mRenderAPI_ref;
		};
	} // namespace core
} // namespace cube
