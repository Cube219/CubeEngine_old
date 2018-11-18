#pragma once

#include "../VulkanAPIHeader.h"

#include "BaseRenderAPI_New/Interface/TextureView.h"

namespace cube
{
	namespace render
	{
		class TextureViewVk final : public TextureView
		{
		public:
			TextureViewVk();
			virtual ~TextureViewVk();
		};
	} // namespace render
} // namespace cuebe
