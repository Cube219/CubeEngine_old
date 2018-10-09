#pragma once

#include "../VulkanAPIHeader.h"

#include "BaseRenderAPI_New/Interface/Buffer.h"

namespace cube
{
	namespace render
	{
		class BufferVk final : public Buffer
		{
		public:
			BufferVk();
			virtual ~BufferVk();

			VkBuffer GetHandle() const { return mBuffer; }

		private:
			VkBuffer mBuffer;
		};
	} // namespace render
} // namespace cube
