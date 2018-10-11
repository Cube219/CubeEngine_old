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
			const char* debugName;
		};

		class Device
		{
		public:
			Device() {}
			virtual ~Device() {}

			virtual SPtr<Buffer> CreateBuffer(const BufferAttribute& attr) = 0;
		};
	} // namespace render
} // namespace cube
