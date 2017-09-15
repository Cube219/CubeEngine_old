#include "StringManager.h"

#include "PathString.h"

namespace cube
{
	namespace core
	{
		StringManager::StringManager()
		{
		}

		StringManager::~StringManager()
		{
		}

		SPtr<PathString> StringManager::GetPathString(WString& string)
		{
			if(mPathStringMap.find(string) == mPathStringMap.end()) {
				mPathStringMap.insert(
				{string, SPtr<PathString>(new PathString(string))}
				);
			}

			return mPathStringMap.find(string)->second;
		}

		SPtr<PathString> StringManager::GetPathString(wchar_t* string)
		{
			return GetPathString(WString(string));
		}
	}
}