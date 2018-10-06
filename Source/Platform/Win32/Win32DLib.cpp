#ifdef _WIN32

#include "Win32DLib.h"

#include <iostream>

#include "../PlatformAssertion.h"
#include "../PlatformString.h"

namespace cube
{
	namespace platform
	{
		Win32DLib::Win32DLib(const String& path)
		{
			PString pathWithExtension = ToPString(path) + L".dll";

			mDLib = LoadLibrary(pathWithExtension.c_str());
			PLATFORM_CHECK(mDLib, "Failed to load a DLib. ({0}.dll) (ErrorCode: {1})", path, GetLastError());
		}

		Win32DLib::~Win32DLib()
		{
			if(mDLib) {
				BOOL r = FreeLibrary(mDLib);
				PLATFORM_CHECK(r, "Failed to unload the DLib. (ErrorCode: {0})", GetLastError());
			}
		}

		void* Win32DLib::GetFunctionImpl(const String& name)
		{
			if(!mDLib)
				return nullptr;

			std::string aName = ToASCIIString(name);
			auto pFunction = GetProcAddress(mDLib, aName.c_str());
			PLATFORM_CHECK(pFunction, "Failed to get the function({0}). (ErrorCode: {1})", name, GetLastError());

			return RCast(void*)(pFunction);
		}
	} // namespace platform
} // namespace cube

#endif // _WIN32
