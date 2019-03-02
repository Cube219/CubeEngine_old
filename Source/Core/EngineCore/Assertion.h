#pragma once

#include "LogWriter.h"
#include "PlatformDebugUtility.h"
#include "Base/format.h"

namespace cube
{
	template <typename ...Args>
	void AssertionFailed(const char* fileName, int lineNum, const String& msg, Args&&... args)
	{
		String str = fmt::format(msg, std::forward<Args>(args)...);
		LogWriter::WriteLogImpl(LogType::Error, fileName, lineNum, str);
		platform::PlatformDebugUtility::AssertionFailed(str, nullptr, fileName, lineNum);
	}

	template <typename ...Args>
	void AssertionFailed(const char* fileName, int lineNum, const Character* msg, Args&&... args)
	{
		String str = fmt::format(msg, std::forward<Args>(args)...);
		LogWriter::WriteLogImpl(LogType::Error, fileName, lineNum, str);
		platform::PlatformDebugUtility::AssertionFailed(str, nullptr, fileName, lineNum);
	}
} // namespace cube

#ifdef _DEBUG

#define ASSERTION_FAILED(msg, ...)                                         \
	cube::AssertionFailed(__FILE__, __LINE__, CUBE_T(msg), ##__VA_ARGS__);

#define CHECK(expr, msg, ...)                \
	if(!(expr)){                             \
		ASSERTION_FAILED(msg, ##__VA_ARGS__) \
	}

#else // _DEBUG

#define ASSERTION_FAILED(msg, ...)
#define CHECK(expr, msg, ...)

#endif // _DEBUG
