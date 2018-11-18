#pragma once

#include "VulkanAPIHeader.h"

#include "BaseRenderAPI_New/Interface/RenderTypes.h"

namespace cube
{
	namespace render
	{
		void TypeConversionInit();

		VkFormat TextureFormatToVkFormat(TextureFormat texFormat);

		static Array<VkFormat, (Uint32)TextureFormat::TextureFormatCount> texFmtToVkFmt;
	} // namespace render
} // namespace cube
