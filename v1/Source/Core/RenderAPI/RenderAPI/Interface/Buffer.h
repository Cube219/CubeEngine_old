#pragma once

#include "../RenderAPIHeader.h"

#include "../Utilities/BaseRenderObject.h"

namespace cube
{
	namespace render
	{
		struct BufferAttribute : public BaseAttribute
		{
			const void* pData;
			Uint64 size = 0;
			bool cpuAccessible = false;
			ResourceUsage usage;
			BufferBindTypeFlags bindTypeFlags;
			bool isDedicated = false;
		};

		class Buffer : public BaseRenderObject
		{
		public:
			Buffer(const BufferAttribute& attr) :
				BaseRenderObject(attr.debugName),
				mUsage(attr.usage), mBindTypeFlags(attr.bindTypeFlags)
			{}
			virtual ~Buffer() {}

			ResourceUsage GetUsage() const { return mUsage; }
			BufferBindTypeFlags GetBindTypeFlags() const { return mBindTypeFlags; }

			virtual void UpdateData(CommandList& cmdList, Uint64 offset, Uint64 size, const void* pData) = 0;
			virtual void CopyData(CommandList& cmdList, const Buffer& src, Uint64 srcOffset,
				Uint64 dstOffset, Uint64 size) = 0;

			virtual void Map(void*& pMappedData) = 0;
			virtual void Unmap() = 0;

			virtual void CreateView() = 0;

		protected:
			ResourceUsage mUsage;
			BufferBindTypeFlags mBindTypeFlags;
		};
	} // namespace render
} // namespace cube
