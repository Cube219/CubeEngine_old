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

			template <typename ...Args>
			static void WriteLog(LogType type, const char* fileName, int lineNum, const String& msg, Args&&... args)
			{
				WriteLogImpl(type, fileName, lineNum, fmt::format(msg, std::forward<Args>(args)...));
			}

			template <typename ...Args>
			static void WriteLog(LogType type, const char* fileName, int lineNum, const Character* msg, Args&&... args)
			{
				WriteLogImpl(type, fileName, lineNum, fmt::format(msg, std::forward<Args>(args)...));
			}

		private:
			LogWriter() = delete;
			LogWriter(const LogWriter& other) = delete;
			//LogWriter& LogWriter::operator=(const LogWriter& other) = delete; // TODO: 왜 에러가?
			
			// Only can access to the friend class (EngineCore)
			static void Init();

			static const char* SplitFileName(const char* fullPath);

			static void WriteLogImpl(LogType type, const char* fileName, int lineNum, String&& msg);
		};
	} // namespace core
} // namespace cube

#define CUBE_LOG(type, msg, ...) cube::core::LogWriter::WriteLog(type, __FILE__, __LINE__, CUBE_T(msg), ##__VA_ARGS__)
