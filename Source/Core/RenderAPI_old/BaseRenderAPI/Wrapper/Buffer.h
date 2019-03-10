#pragma once

#include "../RenderAPIHeader.h"

#include "Base/Flags.h"

namespace cube
{
	namespace render
	{
		enum class BufferTypeFlag
		{
			Uniform = 1,
			Vertex = 2,
			Index = 4,
			TransferSource = 8
		};
		using BufferTypeFlags = Flags<BufferTypeFlag>;

		struct BufferInfo
		{
			SPtr<const Buffer> buffer;
			uint64_t offset;
			uint64_t range;
		};

		struct BufferInitializer
		{
			BufferTypeFlags type;

			struct BufferData
			{
				const void* data;
				uint64_t size;
			};
			Vector<BufferData> bufferDatas;
		};

		class Buffer
		{
		public:
			virtual ~Buffer(){ }

			virtual void Map() = 0;
			virtual void Map(uint64_t startIndex, uint64_t endIndex) = 0;
			virtual void UpdateBufferData(uint64_t index, const void* data, size_t size) = 0;
			virtual void Unmap() = 0;

			virtual BufferInfo GetInfo(uint64_t index) const = 0;

			size_t GetSize() const { return mSize; }

		protected:
			Buffer(){ }

			size_t mSize;
		};
	}
}