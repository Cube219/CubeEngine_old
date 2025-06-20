#include "PlatformAssertion.h"

namespace cube
{
	namespace platform
	{
		const char* SplitFileName(const char* fullPath)
		{
			const char* pitch;

#ifdef _WIN32
			pitch = strrchr(fullPath, '\\');
#else
			pitch = fullPath;
#endif

			if(pitch != nullptr)
				return pitch + 1;

			return fullPath;
		}

		void PrintConsoleImpl(const char* fileName, int lineNum, String&& msg)
		{
			const char* prefix = "PLOG [";

			String res = fmt::format(CUBE_T("{0} / {1}:{2}] : {3}"), prefix, SplitFileName(fileName), lineNum, msg);
			platform::PlatformDebugUtility::PrintToConsole(res);
		}
	} // namespace platform
} // namespace cube
