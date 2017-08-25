#pragma once

#include "..\BaseRenderAPIHeader.h"

namespace cube
{
	namespace core
	{
		class BaseRenderFence
		{
		public:
			virtual ~BaseRenderFence(){ }

			virtual bool Wait(uint64_t timeout) = 0;
			virtual void Reset() = 0;

		protected:
			BaseRenderFence(){ }
		};
	}
}