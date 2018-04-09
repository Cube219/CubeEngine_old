#pragma once

#include "../../EngineCoreHeader.h"

#include "../../Resource/BaseResource.h"
#include "BaseRenderAPI/RenderAPI.h"

namespace cube
{
	namespace core
	{
		struct ShaderCompileDesc
		{
			render::ShaderLanguage language;
			render::ShaderTypeBits type;
			U8String entryPoint;
		};

		class ENGINE_CORE_EXPORT ShaderImporter : public ResourceImporter
		{
		public:
			ShaderImporter(SPtr<render::RenderAPI>& renderAPI) :
				mRenderAPI(renderAPI)
			{
				mResName = CUBE_T("shader");
			}

			Resource* Import(SPtr<platform::File>& file, Json info) final override;

		private:
			ShaderCompileDesc GetCompileDesc(Json& info);

			SPtr<render::RenderAPI> mRenderAPI;
		};

		class ENGINE_CORE_EXPORT Shader : public Resource
		{
		public:
			static RPtr<Shader> Load(const String& path);

		public:
			~Shader();

			render::ShaderLanguage GetLanguage() const { return mLanguage; }
			render::ShaderTypeBits GetType() const { return mType; }

		private:
			friend class ShaderImporter;
			Shader() { }

			friend class RendererManager;
			SPtr<render::Shader> GetRenderShader() const { return mRenderShader; }

			render::ShaderLanguage mLanguage;
			render::ShaderTypeBits mType;

			SPtr<render::Shader> mRenderShader;
		};
	} // namespace core
} // namespace cube
