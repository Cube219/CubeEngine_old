#pragma once

#include "EngineCoreHeader.h"

#include "BasePlatform\BasePlatform.h"

namespace cube
{
	namespace core
	{
		class LogWriter
		{
			friend class EngineCore;
		public:

			static void WriteLog(WString msg);
			static void WriteDebugLog(WString msg);

		private:
			LogWriter() = delete;
			LogWriter(const LogWriter& other) = delete;
			LogWriter& LogWriter::operator=(const LogWriter& other) = delete;
			
			// Only can access to the friend class (EngineCore)
			static void Init(SPtr<platform::BasePlatform>& platform);

			static SPtr<platform::BasePlatform> mPlatform;
		};
	}
}