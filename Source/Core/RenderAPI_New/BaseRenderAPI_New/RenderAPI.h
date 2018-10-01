#pragma once

#include "RenderAPIHeader.h"

namespace cube
{
	namespace render
	{
		struct RenderAPIAttribute
		{
			bool enableDebugLayer;
		};

		class RenderAPI
		{
		public:
			RenderAPI() {}
			virtual ~RenderAPI() {}

			virtual void Init(const RenderAPIAttribute& attr) = 0;

			virtual SPtr<Device> GetDevice(const DeviceAttribute& attr) = 0;
		};
	} // namespace render
} // namespace cube
