#pragma once

#include "../../EngineCoreHeader.h"

#include "../../Resource/BaseResource.h"

#include "RenderAPI/Interface/Device.h"
#include "RenderAPI/Interface/Shader.h"

namespace cube
{
	class ENGINE_CORE_EXPORT Shader : public Resource
	{
	public:
		static RPtr<Shader> Load(StringRef path);

	public:
		Shader() { }
		~Shader() { }

		render::ShaderLanguage GetLanguage() const { return mLanguage; }
		render::ShaderType GetType() const { return mType; }

		void _LoadShader(SPtr<render::Device>& device, render::ShaderAttribute& attr);

		SPtr<render::Shader> GetRenderShader() const { return mRenderShader; }

	private:
		render::ShaderLanguage mLanguage;
		render::ShaderType mType;

		SPtr<render::Shader> mRenderShader;
	};
} // namespace cube
