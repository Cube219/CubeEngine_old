#include "LogWriter.h"

#include <iostream>
#include <string.h>

namespace cube
{
	namespace core
	{
		void LogWriter::WriteLog(LogType type, WString msg, const char* fileName, int lineNum)
		{
			// INFO [18:22:22.1111 / EngineCore.cpp:152] : 내용
			
			const wchar_t* prefix = L"";

			switch(type) {
				case LogType::Info:
					prefix = L"   INFO [";
					break;
				case LogType::Warning:
					prefix = L"WARNING [";
					break;
				case LogType::Error:
					prefix = L"  ERROR [";
					break;

				default:
					break;
			}

			std::wcout << prefix << " / " << SplitFileName(fileName) << ":" << lineNum << "] : " << msg << std::endl;
		}

		void LogWriter::Init()
		{
			// TODO: File에 쓰는 방식 추가
			// TODO: 시간 기록
		}

		const char* LogWriter::SplitFileName(const char* fullPath)
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
	}
}
