#ifdef _WIN32

#include "Win32DebugUtility.h"

#include <signal.h>
#include <iostream>
#include "../PlatformString.h"

namespace cube
{
	namespace platform
	{
		std::mutex Win32DebugUtility::printMutex;

		void Win32DebugUtility::AssertionFailedImpl(StringRef msg, const char* funcName, const char* fileName, int line)
		{
			String str = fmt::format(CUBE_T("Assertion failed!\n\n[{0}:{1}] : {2}\n\n(Press Retry to debug the application)"),
				GetBaseName(fileName), line, msg.GetString());
			PString pMsg = ToPString(str);

			int nCode = MessageBox(NULL, pMsg.c_str(), L"Assertion failed",
				MB_TASKMODAL | MB_ICONHAND | MB_ABORTRETRYIGNORE | MB_SETFOREGROUND);

			switch(nCode) {
			case IDABORT:
				raise(SIGABRT);

				exit(3);
				break;

			case IDRETRY:
				DebugBreak();
				break;

			case IDIGNORE:
				break;
			}
		}

		void Win32DebugUtility::PrintToConsoleImpl(StringRef str)
		{
			PString pStr = ToPString(str.GetString());

			std::unique_lock<std::mutex> lock(printMutex);
			std::wcout << pStr << std::endl;
		}

		const char* Win32DebugUtility::GetBaseName(const char* absolutePath)
		{
			const char* pitch;

#ifdef _WIN32
			pitch = strrchr(absolutePath, '\\');
#else
			pitch = fullPath;
#endif

			if(pitch != nullptr)
				return pitch + 1;

			return absolutePath;
		}
	} // namespace platform
} // namespace cube

#endif // _WIN32
