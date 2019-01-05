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

			void* GetFunction(StringRef name);
		};

#define DLIB_DEFINITION(Child)                                 \
		inline void* DLib::GetFunction(StringRef name) {   \
			return RCast(Child*)(this)->GetFunctionImpl(name); \
		}
	} // namespace platform
} // namespace cube
