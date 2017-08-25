#include "BasePlatform\BasePlatformLogWriter.h"

namespace cube
{
	namespace platform
	{
		void WriteLog(std::wstring log)
		{
			std::wcout << log << std::endl;
		}

		void WriteDebugLog(std::wstring log)
		{
			std::wcout << log << std::endl;
		}

		void WriteErrorLog(std::wstring log)
		{
			std::wcout << log << std::endl;
		}
	}
}