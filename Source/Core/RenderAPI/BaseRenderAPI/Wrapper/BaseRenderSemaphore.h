#pragma once

#include "../BaseRenderAPIHeader.h"

namespace cube
{
	namespace core
	{
		class BaseRenderSemaphore
		{
		public:
			virtual ~BaseRenderSemaphore(){ }

		protected:
			BaseRenderSemaphore(){ }
		};
	}
}