#pragma once

#include "../VulkanAPIHeader.h"

#include "BaseRenderAPI_New/Interface/BufferView.h"

namespace cube
{
	namespace render
	{
		// TODO: 나중에 구현
		class BufferViewVk final : public BufferView
		{
		public:
			BufferViewVk();
			virtual ~BufferViewVk();
		};
	} // namespace render
} // namespace cube
