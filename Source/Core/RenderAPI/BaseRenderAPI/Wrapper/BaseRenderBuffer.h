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

		struct BaseRenderBufferInfo
		{
			WPtr<BaseRenderBuffer> buffer;
			uint64_t offset;
			uint64_t range;
		};

		struct BaseRenderBufferInitializer
		{
			BufferTypeBits type;

			struct BufferData
			{
				const void* data;
				uint64_t size;
			};
			Vector<BufferData> bufferDatas;
		};

		class BaseRenderBuffer
		{
		public:
			virtual ~BaseRenderBuffer(){ }

			virtual void Map() = 0;
			virtual void UpdateBufferData(uint64_t index, const void* data, size_t size) = 0;
			virtual void Unmap() = 0;

			virtual BaseRenderBufferInfo GetInfo(uint64_t index) const = 0;

			size_t GetSize() const { return mSize; }

		protected:
			BaseRenderBuffer(){ }

			size_t mSize;
		};
	}
}