#include "Shader.h"

#include "../../EngineCore.h"
#include "../RendererManager.h"
#include "BaseRenderAPI/Wrapper/BaseRenderDescriptor.h"

namespace cube
{
	namespace core
	{
		Shader::Shader(ResourceRawData* rawData) : 
			BaseResource(rawData)
		{
			char* p = (char*)rawData->GetRawData();
			p[rawData->GetSize()] = '\0';
		}

		Shader::~Shader()
		{
		}

		void Shader::Complie(SPtr<BaseRenderAPI> renderAPI, ShaderComplieDesc& desc)
		{
			mLanguage = desc.language;
			mType = desc.type;
			
			BaseRenderShaderInitializer shaderInit;
			shaderInit.language = desc.language;
			shaderInit.type = desc.type;
			shaderInit.entryPoint = desc.entryPoint.c_str();
			shaderInit.code = (const char*)(mRawData->GetRawData());
			mRenderShader = renderAPI->CreateShader(shaderInit);
		}
	}
}