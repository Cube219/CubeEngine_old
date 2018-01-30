#pragma once

#include "../../EngineCoreHeader.h"

#include "../../Resource/BaseResource.h"
#include "BaseRenderAPI/BaseRenderAPI.h"

namespace cube
{
	namespace core
	{
		struct ShaderComplieDesc
		{
			ShaderLanguage language;
			ShaderTypeBits type;
			String entryPoint;
		};

		class ENGINE_CORE_EXPORT Shader : public BaseResource
		{
		public:
			~Shader();

			ShaderLanguage GetLanguage() const { return mLanguage; }
			ShaderTypeBits GetType() const { return mType; }

			void Complie(SPtr<BaseRenderAPI> renderAPI, ShaderComplieDesc& desc);

		private:
			friend class BaseResource;
			Shader(ResourceRawData* rawData);

			friend class RendererManager;
			SPtr<BaseRenderShader> GetRenderShader() const { return mRenderShader; }

			ShaderLanguage mLanguage;
			ShaderTypeBits mType;

			SPtr<BaseRenderShader> mRenderShader;
		};
	}
}