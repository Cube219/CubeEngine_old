#pragma once

#include "PlatformHeader.h"

namespace cube
{
	namespace platform
	{
		class PLATFORM_EXPORT DLib
		{
		public:
			DLib(){ }
			virtual ~DLib(){ }

			void* GetFunction(const String2& name);

		protected:
			struct Data;
			Data* mData;
		};
	}
}
