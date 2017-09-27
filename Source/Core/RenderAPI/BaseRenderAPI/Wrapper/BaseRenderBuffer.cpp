#include "BaseRenderBuffer.h"

namespace cube
{
	namespace core
	{
		BaseRenderBufferInfo BaseRenderBuffer::GetInfo(uint64_t offset, uint64_t range) const
		{
			BaseRenderBufferInfo info;
			info.buffer = shared_from_this();
			info.offset = offset;
			info.range = range;

			return info;
		}
	}
}