#pragma once

#include "../CubeEngineHeader.h"

#include "EngineCore/Resource/BaseResource.h"
#include "RenderAPI/Interface/Device.h"

namespace cube
{
	struct ShaderCompileDesc
	{
		render::ShaderLanguage language;
		render::ShaderType type;
		U8String entryPoint;
	};

	class ShaderImporter : public ResourceImporter
	{
	public:
		ShaderImporter(SPtr<render::Device>& device) :
			mDevice(device)
		{
			mName = CUBE_T("ShaderImporter");
		}

		Resource* Import(SPtr<platform::File>& file, Json info) final override;

	private:
		ShaderCompileDesc GetCompileDesc(Json& info);

		SPtr<render::Device> mDevice;
	};
} // namespace cube
