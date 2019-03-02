#pragma once

#include "../CubeEngineHeader.h"

#include "EngineCore/Resource/BaseResource.h"
#include "RenderAPI/Interface/Device.h"

namespace cube
{
	class TextureImporter : public ResourceImporter
	{
	public:
		TextureImporter(SPtr<render::Device>& device) :
			mDevice(device)
		{
			mName = CUBE_T("TextureImporter");
		}

		Resource* Import(SPtr<platform::File>& file, Json info) final override;

	private:
		SPtr<TextureData> GetTextureData(void* pData, Uint32 width, Uint32 height, Uint32 depth);

		SPtr<render::Device> mDevice;
	};
} // namespace cube
