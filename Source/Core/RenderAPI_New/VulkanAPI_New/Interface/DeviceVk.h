#pragma once

#include "../VulkanAPIHeader.h"

#include "BaseRenderAPI_New/Interface/Device.h"

namespace cube
{
	namespace render
	{
		class DeviceVk final : public Device
		{
			DeviceVk(DeviceAttribute& attr);
		};
	} // namespace render
} // namespace cube
