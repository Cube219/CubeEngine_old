#pragma once

#include <string>
#include <iostream>

namespace cube
{
	namespace platform
	{
		void WriteLog(std::wstring log);
		void WriteDebugLog(std::wstring log);
		void WriteErrorLog(std::wstring log);
	}
}