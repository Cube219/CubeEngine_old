#pragma once

#include "../../EngineCoreHeader.h"

#include "../../Resource/BaseResource.h"
/*
#include "BaseRenderAPI/RenderAPI.h"
#include "BaseRenderAPI/Wrapper/Shader.h"
*/
#include "BaseRenderAPI_New/Interface/Device.h"
#include "BaseRenderAPI_New/Interface/Shader.h"

namespace cube
{
	namespace core
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

		private:
			friend class RendererManager;
			SPtr<render::Shader> GetRenderShader() const { return mRenderShader; }

			render::ShaderLanguage mLanguage;
			render::ShaderType mType;

			SPtr<render::Shader> mRenderShader;
		};
	} // namespace core
} // namespace cube
