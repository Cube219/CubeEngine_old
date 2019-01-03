#include "Shader.h"

#include "../../EngineCore.h"
#include "../../Resource/ResourceManager.h"

namespace cube
{
	namespace core
	{
		RPtr<Shader> Shader::Load(const String& path)
		{
			return ECore().GetResourceManager().LoadResource<Shader>(path);
		}

		void Shader::_LoadShader(SPtr<render::RenderAPI>& renderAPI, render::ShaderInitializer& init)
		{
			mLanguage = init.language;
			mType = init.type;
			mRenderShader = renderAPI->CreateShader(init);
		}
	} // namespace render
} // namespace cube
