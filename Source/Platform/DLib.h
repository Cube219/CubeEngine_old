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

			void* GetFunction(const String& name);

		protected:
			struct Data;
			Data* mData;
		};
	} // namespace platform
} // namespace cube
