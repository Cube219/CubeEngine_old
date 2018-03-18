#pragma once

#include "EngineCoreHeader.h"

#include "Base/format.h"

namespace cube
{
	enum class LogType
	{
		Info, Warning, Error
	};

	namespace core
	{
		class ENGINE_CORE_EXPORT LogWriter
		{
			friend class EngineCore;
		public:

			static void WriteLog(LogType type, WString msg, const char* fileName, int lineNum);

		private:
			LogWriter() = delete;
			LogWriter(const LogWriter& other) = delete;
			//LogWriter& LogWriter::operator=(const LogWriter& other) = delete; // TODO: 왜 에러가?
			
			// Only can access to the friend class (EngineCore)
			static void Init();

			static const char* SplitFileName(const char* fullPath);
		};
	}
}

#define CUBE_LOG(type, msg, ...) cube::core::LogWriter::WriteLog(type, fmt::format(msg, ##__VA_ARGS__), __FILE__, __LINE__)
