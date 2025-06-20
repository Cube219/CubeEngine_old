#include "Shader.h"

#include "../../EngineCore.h"
#include "../../Resource/ResourceManager.h"

namespace cube
{
	RPtr<Shader> Shader::Load(StringRef path)
	{
		return ECore().GetResourceManager().LoadResource<Shader>(path);
	}

	void Shader::_LoadShader(SPtr<render::Device>& device, render::ShaderAttribute& attr)
	{
		mLanguage = attr.language;
		mType = attr.type;

		attr.debugName = "Shader";

		mRenderShader = device->CreateShader(attr);
	}
} // namespace cube
