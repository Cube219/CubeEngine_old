#pragma once

#include "../CubeEngineHeader.h"

#include "EngineCore/Resource/BaseResource.h"
#include "BaseRenderAPI/RenderAPI.h"

namespace cube
{
	struct ShaderCompileDesc
	{
		render::ShaderLanguage language;
		render::ShaderTypeBits type;
		U8String entryPoint;
	};

	class ShaderImporter : public core::ResourceImporter
	{
	public:
		ShaderImporter(SPtr<render::RenderAPI>& renderAPI) :
			mRenderAPI(renderAPI)
		{
			mName = CUBE_T("ShaderImporter");
		}

		core::Resource* Import(SPtr<platform::File>& file, Json info) final override;

	private:
		ShaderCompileDesc GetCompileDesc(Json& info);

		SPtr<render::RenderAPI> mRenderAPI;
	};
} // namespace cube