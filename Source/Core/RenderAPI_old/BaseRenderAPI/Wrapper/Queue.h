#pragma once

#include "../RenderAPIHeader.h"

namespace cube
{
	namespace render
	{
		enum class QueueTypeBits
		{
			GraphicsBit = 1,
			ComputeBit = 2,
			TransferBit = 4,
			SparseBindingBit = 8
		};
		SET_ENUM_AS_FLAGS(QueueTypeBits)

		class Queue
		{
		public:
			virtual ~Queue(){ }

			QueueTypeBits GetTypes() const { return mTypes; }
			uint32_t GetIndex() const { return mIndex; }

		protected:
			Queue(){ }

			QueueTypeBits mTypes;
			uint32_t mIndex;
		};
	} // namespace render
} // namespace cube
