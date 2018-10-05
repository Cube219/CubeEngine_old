#pragma once

#include "PlatformHeader.h"

namespace cube
{
	namespace platform
	{
		class PLATFORM_EXPORT PlatformDebugUtility
		{
		public:
			static void AssertionFailed(const String& msg, const char* funcName, const char* fileName, int line);
		};
	} // namespace platform
} // namespace cube
