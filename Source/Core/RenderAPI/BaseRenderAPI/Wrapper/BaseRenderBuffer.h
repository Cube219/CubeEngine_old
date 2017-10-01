#pragma once

#include "../BaseRenderAPIHeader.h"

namespace cube
{
	namespace core
	{
		enum class BufferTypeBits
		{
			Uniform = 1,
			Vertex = 2,
			Index = 4,
			TransferSource = 8
		};
		SET_ENUM_AS_FLAGS(BufferTypeBits)

		struct BaseRenderBufferInfo; // Defined at BaseRenderAPIHeader.h

		class BaseRenderBuffer : public std::enable_shared_from_this<BaseRenderBuffer>
		{
		public:
			virtual ~BaseRenderBuffer(){ }

			virtual void Map() = 0;
			virtual void UpdateBufferData(const void* data, size_t size, uint64_t offset) = 0;
			virtual void Unmap() = 0;

			virtual BaseRenderBufferInfo GetInfo(uint64_t offset, uint64_t range) const;

		protected:
			BaseRenderBuffer(){ }
		};
	}
}