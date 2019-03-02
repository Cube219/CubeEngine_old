#pragma once

#ifdef ENGINE_CORE_EXPORTS
#define ENGINE_CORE_EXPORT __declspec(dllexport) 
#else // ENGINE_CORE_EXPORTS
#define ENGINE_CORE_EXPORT __declspec(dllimport) 
#endif // ENGINE_CORE_EXPORTS

#include "Base/BaseTypes.h"
#include "Base/format.h"

namespace cube
{
	enum class LogType
	{
		Info, Warning, Error
	};

	class ENGINE_CORE_EXPORT LogWriter
	{
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
		LogWriter& operator=(const LogWriter& other) = delete;
			
		friend class EngineCore;

		template <typename ...Args>
		friend void AssertionFailed(const char* fileName, int lineNum, const String* msg, Args&&... args);
		template <typename ...Args>
		friend void AssertionFailed(const char* fileName, int lineNum, const Character* msg, Args&&... args);

		static void WriteLogImpl(LogType type, const char* fileName, int lineNum, const String& msg);
		static void WriteLogImpl(LogType type, const char* fileName, int lineNum, String&& msg);
		static void WriteLogImpl(const String& log);

		// Only can access to the friend class (EngineCore)
		static void Init();

		static const char* SplitFileName(const char* fullPath);

	};
} // namespace cube

#define CUBE_LOG(type, msg, ...) cube::LogWriter::WriteLog(type, __FILE__, __LINE__, CUBE_T(msg), ##__VA_ARGS__)
