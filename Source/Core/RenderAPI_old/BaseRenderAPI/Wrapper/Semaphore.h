#pragma once

#include "../RenderAPIHeader.h"

namespace cube
{
	namespace render
	{
		class Semaphore
		{
		public:
			virtual ~Semaphore(){ }

		protected:
			Semaphore(){ }
		};
	} // namespace render
} // namespace cube
