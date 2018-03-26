#include "LogWriter.h"

#include "PlatformString.h"

namespace cube
{
	namespace core
	{
		void LogWriter::Init()
		{
			// TODO: File�� ���� ��� �߰�
			// TODO: �ð� ���
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

		void LogWriter::WriteLogImpl(LogType type, const char* fileName, int lineNum, String2&& msg)
		{
			// INFO [18:22:22.1111 / EngineCore.cpp:152] : ����

			const char* prefix = "";

			switch(type) {
				case LogType::Info:
					prefix = "   INFO [";
					break;
				case LogType::Warning:
					prefix = "WARNING [";
					break;
				case LogType::Error:
					prefix = "  ERROR [";
					break;

				default:
					break;
			}

			String2 res = fmt::format(CUBE_T("{0} / {1}:{2}] : {3}"), prefix, SplitFileName(fileName), lineNum, msg);
#ifdef _DEBUG
			PrintToConsole(res);
#endif // _DEBUG
		}
	} // namespace core
} // namespace cube
