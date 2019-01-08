#pragma once

#include "../PlatformHeader.h"

#include <Windows.h>
#include <mutex>

#include "../PlatformDebugUtility.h"

namespace cube
{
	namespace platform
	{
		class PLATFORM_EXPORT Win32DebugUtility : public PlatformDebugUtility
		{
		public:
			static void AssertionFailedImpl(StringRef msg, const char* funcName, const char* fileName, int lineNum);

			static void PrintToConsoleImpl(StringRef str);

		private:
			static const char* GetBaseName(const char* absolutePath);

			static std::mutex printMutex;
		};
		PLATFORM_DEBUG_UTILITY_DEFINITION(Win32DebugUtility)
	} // namespace platform
} // namespace cube
