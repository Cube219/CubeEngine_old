#include "ShaderImporter.h"

#include "EngineCore/Renderer/Material/Shader.h"
#include "RenderAPI/Interface/Shader.h"

namespace cube
{
	Resource* ShaderImporter::Import(SPtr<platform::File>& file, Json info)
	{
		uint64_t size = file->GetFileSize();

		char* rawData = (char*)malloc(size + 1);
		uint64_t readSize = 0;
		file->Read(rawData, size, readSize);
		rawData[size] = '\0';

		ShaderCompileDesc desc = GetCompileDesc(info);

		render::ShaderAttribute shaderInit;
		shaderInit.language = desc.language;
		shaderInit.type = desc.type;
		shaderInit.entryPoint = desc.entryPoint.c_str();
		shaderInit.code = rawData;

		Shader* shader = new Shader();
		shader->_LoadShader(mDevice, shaderInit);

		free(rawData);

		return shader;
	}

	ShaderCompileDesc ShaderImporter::GetCompileDesc(Json& info)
	{
		using namespace render;

		ShaderCompileDesc desc;

		U8String language = info["language"];
		U8String type = info["type"];
		U8String entryPoint = info["entry_point"];

		if(language == "glsl") {
			desc.language = ShaderLanguage::GLSL;
		} else if(language == "hsls") {
			desc.language = ShaderLanguage::HLSL;
		} else if(language == "spir-v") {
			desc.language = ShaderLanguage::SPIRV;
		}

		if(type == "vertex") {
			desc.type = ShaderType::Vertex;
		} else if(type == "fragment") {
			desc.type = ShaderType::Pixel;
		} else if(type == "pixel") {
			desc.type = ShaderType::Pixel;
		} else if(type == "compute") {
			desc.type = ShaderType::Compute;
		}

		desc.entryPoint = entryPoint;

		return desc;
	}
} // namespace cube
