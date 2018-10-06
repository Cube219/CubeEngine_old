#ifdef _WIN32
/*
#include "../Path.h"

namespace cube
{
	namespace platform
	{
		Path::Path() : 
			mIsFile(false), mIsDirectory(false), mIsAbsolute(false)
		{
		}
		Path::Path(const Character* pathStr)
		{
			String tempStr(pathStr);
			Assign(ToPString(tempStr).c_str());
		}

		Path::Path(const String& pathStr)
		{
			Assign(ToPString(pathStr).c_str());
		}

		Path::Path(const PCharacter* pathStr)
		{
			Assign(pathStr);
		}

		Path::Path(const PString& pathStr)
		{
			Assign(pathStr.c_str());
		}

		void Path::Assign(const PCharacter* pathStr)
		{
			int lastIndex = 0;
			int currentIndex = 0;

			while(pathStr[currentIndex] != L'\0') {
				if(pathStr[currentIndex] == L'\\') {

				}
			}
		}

		PString Path::GetDirectoryName()
		{
			return PString();
		}

		Path Path::GetAbsolute() const
		{
			return Path();
		}

		Path Path::GetRelative() const
		{
			return Path();
		}

		Path Path::GetParent() const
		{
			return Path();
		}

		void Path::Absolute()
		{
		}

		void Path::Relative()
		{
		}

		void Path::Parent()
		{
		}

		bool Path::IsExist() const
		{
			return false;
		}
	} // namespace platform
} // namespace cube
*/
#endif // _WIN32