#pragma once

#include "../../EngineCoreHeader.h"

#include "../../Resource/BaseResource.h"
#include "BaseRenderAPI/BaseRenderAPI.h"

namespace cube
{
	namespace core
	{
		struct ShaderCompileDesc
		{
			ShaderLanguage language;
			ShaderTypeBits type;
			String entryPoint;
		};

		class ENGINE_CORE_EXPORT ShaderImporter : public ResourceImporter
		{
		public:
			ShaderImporter(SPtr<BaseRenderAPI>& renderAPI) :
				mRenderAPI(renderAPI)
			{
				mResName = "shader";
			}

			Resource* Import(SPtr<platform::BasePlatformFile>& file, Json info) final override;

		private:
			ShaderCompileDesc GetCompileDesc(Json& info);

			SPtr<BaseRenderAPI> mRenderAPI;
		};

		class ENGINE_CORE_EXPORT Shader : public Resource
		{
		public:
			~Shader();

			ShaderLanguage GetLanguage() const { return mLanguage; }
			ShaderTypeBits GetType() const { return mType; }

		private:
			friend class ShaderImporter;
			Shader() { }

			friend class RendererManager;
			SPtr<BaseRenderShader> GetRenderShader() const { return mRenderShader; }

			ShaderLanguage mLanguage;
			ShaderTypeBits mType;

			SPtr<BaseRenderShader> mRenderShader;
		};
	}
}