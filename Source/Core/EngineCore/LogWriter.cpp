#include "LogWriter.h"

#include <iostream>

namespace cube
{
	namespace core
	{
		SPtr<platform::BasePlatform> LogWriter::mPlatform;

		void LogWriter::WriteLog(WString msg)
		{
			std::wcout << msg << std::endl;
		}

		void LogWriter::WriteDebugLog(WString msg)
		{
#ifdef _DEBUG
			std::wcout << msg << std::endl;
#endif // _DEBUG
		}

		void LogWriter::Init(SPtr<platform::BasePlatform>& platform)
		{
			// TODO: File에 쓰는 방식 추가
			// TODO: 시간 기록
			mPlatform = platform;
		}
	}
}