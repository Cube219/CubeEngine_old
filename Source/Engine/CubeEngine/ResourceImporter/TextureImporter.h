#pragma once

#include "../CubeEngineHeader.h"

#include "EngineCore/Resource/BaseResource.h"
#include "BaseRenderAPI_New/Interface/Device.h"

namespace cube
{
	class TextureImporter : public core::ResourceImporter
	{
	public:
		TextureImporter(SPtr<render::Device>& device) :
			mDevice(device)
		{
			mName = CUBE_T("TextureImporter");
		}

		core::Resource* Import(SPtr<platform::File>& file, Json info) final override;

	private:
		SPtr<render::Device> mDevice;
	};
} // namespace cube
