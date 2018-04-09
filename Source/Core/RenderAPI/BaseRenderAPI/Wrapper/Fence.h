#pragma once

#include "../RenderAPIHeader.h"

namespace cube
{
	namespace render
	{
		class Fence
		{
		public:
			virtual ~Fence(){ }

			virtual bool Wait(uint64_t timeout) = 0;
			virtual void Reset() = 0;

		protected:
			Fence(){ }
		};
	} // namespace render
} // namespace cube
