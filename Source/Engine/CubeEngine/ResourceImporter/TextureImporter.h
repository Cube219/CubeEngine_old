#pragma once

#include "../CubeEngineHeader.h"

#include "EngineCore/Resource/BaseResource.h"
#include "BaseRenderAPI/RenderAPI.h"

namespace cube
{
	class TextureImporter : public core::ResourceImporter
	{
	public:
		TextureImporter(SPtr<render::RenderAPI>& renderAPI) :
			mRenderAPI(renderAPI)
		{
			mName = CUBE_T("TextureImporter");
		}

		core::Resource* Import(SPtr<platform::File>& file, Json info) final override;

	private:
		SPtr<render::RenderAPI> mRenderAPI;
	};
} // namespace cube
