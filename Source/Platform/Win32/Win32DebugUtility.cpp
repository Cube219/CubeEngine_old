#include "Win32DebugUtility.h"

namespace cube
{
	namespace platform
	{
		void PlatformDebugUtility::AssertionFailed(const String& msg, const char* funcName, const char* fileName, int line)
		{
			String msg = fmt::format(CUBE_T("Assertion failed! "));
		}
	} // namespace platform
} // namespace cube
