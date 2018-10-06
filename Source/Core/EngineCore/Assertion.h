#pragma once

#include "LogWriter.h"
#include "PlatformDebugUtility.h"
#include "Base/format.h"

namespace cube
{
	namespace core
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
	} // namespace core
} // namespace cube

#ifdef _DEBUG

#define CHECK(expr, msg, ...)                                                        \
	if(!(expr)){                                                                     \
		cube::core::AssertionFailed(__FILE__, __LINE__, CUBE_T(msg), ##__VA_ARGS__); \
	}

#define CHECK_LITE(expr, msg, ...)                                                           \
	if(!(expr)) {                                                                            \
		cube::String str = fmt::format(CUBE_T("Assertion_Lite Failed! : {0}"), CUBE_T(msg)); \
		CUBE_LOG(cube::LogType::Warning, str, ##__VA_ARGS__);                                \
	}

#else // _DEBUG

#define CHECK(expr, msg, ...)
#define CHECK_LITE(expr, msg, ...)

#endif // _DEBUG