#pragma once

#include "..\EngineCoreHeader.h"

#include <unordered_map>

namespace cube
{
	namespace core
	{
		class ENGINE_CORE_EXPORT StringManager
		{
		public:
			StringManager();
			~StringManager();

			SPtr<PathString> GetPathString(WString& string);
			SPtr<PathString> GetPathString(wchar_t* string);

		private:
			std::unordered_map<WString, SPtr<PathString>> mPathStringMap;
		};
	}
}