#pragma once

#include "../RenderAPIHeader.h"

namespace cube
{
	namespace render
	{
		struct DeviceAttribute
		{
			uint32_t GPUIndex;
			bool enableDebugLayer;
		};

		class Device
		{
		public:
			Device() {}
			virtual ~Device() {}
		};
	} // namespace render
} // namespace cube