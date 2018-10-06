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

#define CHECK(expr, msg, ...)                                                            \
	if(!(expr)){                                                                         \
		cube::platform::AssertionFailed(__FILE__, __LINE__, CUBE_T(msg), ##__VA_ARGS__); \
	}

#define CHECK_LITE(expr, msg, ...)                                                    \
	if(!(expr)) {                                                                     \
		cube::platform::PrintConsole(__FILE__, __LINE__, CUBE_T(msg), ##__VA_ARGS__); \
	}

#else // _DEBUG

#define CHECK(expr, msg, ...)
#define CHECK_LITE(expr, msg, ...)

#endif // _DEBUG
