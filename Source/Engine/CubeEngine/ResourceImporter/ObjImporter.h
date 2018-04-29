#pragma once

#include "../CubeEngineHeader.h"

#include "EngineCore/Resource/BaseResource.h"

namespace cube
{
	class ObjImporter : public core::ResourceImporter
	{
	public:
		ObjImporter()
		{
			mName = CUBE_T("ObjImporter");
		}

		core::Resource* Import(SPtr<platform::File>& file, Json info) final override;
	};
} // namespace cube
