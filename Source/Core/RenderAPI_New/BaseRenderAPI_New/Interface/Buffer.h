#pragma once

#include "../RenderAPIHeader.h"

namespace cube
{
	namespace render
	{
		enum class BufferUsage
		{
			Default,
			Immutable,
			Dynamic,
			Staging
		};

		using BufferBindTypeFlags = Uint32;
		enum BufferBindTypeFlagsBit : Uint32
		{
			Vertex = 1,
			Index = 2,
			Uniform = 4,
			TransferSource = 8,
			TransferDest = 16
		};

		struct BufferData
		{
			const void* pData;
			Uint64 dataSize;
		};

		struct BufferAttribute
		{
			Uint64 size = 0;
			bool cpuAccessible = false;
			BufferUsage usage;
			BufferBindTypeFlags bindTypeFlags = 0;
			const BufferData* pData = nullptr;
			const char* debugName = nullptr;
		};

		class Buffer
		{
		public:
			Buffer() {}
			virtual ~Buffer() {}

			virtual void UpdateData(Uint64 offset, Uint64 size, const void* pData) = 0;
			virtual void CopyData(const SPtr<Buffer>& src, Uint64 srcOffset, Uint64 dstOffset, Uint64 size) = 0;

			virtual void Map(void*& pMappedData) = 0;
			virtual void Unmap() = 0;

			virtual void CreateView() = 0;

		protected:
			BufferUsage mUsage;
		};
	} // namespace render
} // namespace cube
