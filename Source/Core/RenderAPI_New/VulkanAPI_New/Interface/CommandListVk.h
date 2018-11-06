#pragma once

#include "../VulkanAPIHeader.h"

#include "BaseRenderAPI_New/Interface/CommandList.h"

namespace cube
{
	namespace render
	{
		class CommandListVk final : public CommandList
		{
		public:
			CommandListVk();
			virtual ~CommandListVk();
		};
	} // namespace render
} // namespace cube
