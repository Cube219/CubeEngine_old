#pragma once

#include "Base\BaseTypes.h"

namespace cube
{
	namespace platform
	{
		class BasePlatformDLib
		{
		public:
			BasePlatformDLib(){ }
			virtual ~BasePlatformDLib(){ }

			virtual void* GetFunction(String name) = 0;
		};
	}
}