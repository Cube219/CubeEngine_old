#pragma once

#include "PlatformHeader.h"

namespace cube
{
	namespace platform
	{
		class PLATFORM_EXPORT PlatformDebugUtility
		{
		public:
			static void AssertionFailed(StringRef msg, const char* funcName, const char* fileName, int lineNum);

			static void PrintToConsole(StringRef str);
		};

#define PLATFORM_DEBUG_UTILITY_DEFINITION(Child)                                           \
		inline void PlatformDebugUtility::AssertionFailed                                  \
			(StringRef msg, const char* funcName, const char* fileName, int lineNum) { \
			Child::AssertionFailedImpl(msg, funcName, fileName, lineNum);                  \
		}                                                                                  \
                                                                                           \
		inline void PlatformDebugUtility::PrintToConsole(StringRef str)                \
		{                                                                                  \
			Child::PrintToConsoleImpl(str);                                                \
		}
	} // namespace platform
} // namespace cube
