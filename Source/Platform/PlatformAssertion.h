#pragma once

// Only use in Platform project

#include "PlatformDebugUtility.h"
#include "Base/format.h"

namespace cube
{
	namespace platform
	{
		template <typename ...Args>
		void AssertionFailed(const char* fileName, int lineNum, const String& msg, Args&&... args)
		{
			platform::PlatformDebugUtility::AssertionFailed(fmt::format(msg, std::forward<Args>(args)...), nullptr, fileName, lineNum);
		}

		template <typename ...Args>
		void AssertionFailed(const char* fileName, int lineNum, const Character* msg, Args&&... args)
		{
			platform::PlatformDebugUtility::AssertionFailed(fmt::format(msg, std::forward<Args>(args)...), nullptr, fileName, lineNum);
		}

		void PrintConsoleImpl(const char* fileName, int lineNum, String&& msg);
		template <typename ...Args>
		void PrintConsole(const char* fileName, int lineNum, const String& msg, Args&&... args)
		{
			PrintConsoleImpl(fileName, lineNum, fmt::format(msg, std::forward<Args>(args)...));
		}

		template <typename ...Args>
		void PrintConsole(const char* fileName, int lineNum, const Character* msg, Args&&... args)
		{
			PrintConsoleImpl(fileName, lineNum, fmt::format(msg, std::forward<Args>(args)...));
		}
	} // namespace core
} // namespace cube

#ifdef _DEBUG

#define PLATFORM_ASSERTION_FAILED(msg, ...)                                          \
	cube::platform::AssertionFailed(__FILE__, __LINE__, CUBE_T(msg), ##__VA_ARGS__);

#define PLATFORM_CHECK(expr, msg, ...)                 \
	if(!(expr)){                                       \
		PLATFORM_ASSERTION_FAILED(msg, ##__VA_ARGS__); \
	}

#define PLATFORM_PRINT_LOG(msg, ...)                                              \
	cube::platform::PrintConsole(__FILE__, __LINE__, CUBE_T(msg), ##__VA_ARGS__);

#else // _DEBUG

#define PLATFORM_ASSERTION_FAILED(msg, ...)
#define PLATFORM_CHECK(expr, msg, ...)
#define PLATFORM_PRINT_LOG(msg, ...)

#endif // _DEBUG
