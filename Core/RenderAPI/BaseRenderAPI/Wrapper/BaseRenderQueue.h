#pragma once

#include "..\BaseRenderAPIHeader.h"

namespace cube
{
	namespace core
	{
		enum class QueueTypeBits
		{
			GraphicsBit = 1,
			ComputeBit = 2,
			TransferBit = 4,
			SparseBindingBit = 8
		};
		SET_ENUM_AS_FLAGS(QueueTypeBits)

		class BaseRenderQueue
		{
		public:
			virtual ~BaseRenderQueue(){ }

		protected:
			BaseRenderQueue(){ }
		};
	}
}