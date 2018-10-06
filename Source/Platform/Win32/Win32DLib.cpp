#ifdef _WIN32

#include "Win32DLib.h"

#include <iostream>

#include "../PlatformString.h"

namespace cube
{
	namespace platform
	{
		Win32DLib::Win32DLib(const String& path)
		{
			PString pathWithExtension = ToPString(path) + L".dll";

			mDLib = LoadLibrary(pathWithExtension.c_str());

			if(mDLib == NULL) {
				std::wcout << L"Win32DLib: Failed to load a DLib. (" << pathWithExtension << L")" << std::endl;
			}
		}

		Win32DLib::~Win32DLib()
		{
			if(mDLib) {
				BOOL r = FreeLibrary(mDLib);
				if(r == 0) {
					std::wcout << L"Win32DLib: Failed to unload the DLib." << std::endl;
				}
			}
		}

		void* Win32DLib::GetFunctionImpl(const String& name)
		{
			if(!mDLib)
				return nullptr;

			std::string aName = ToASCIIString(name);
			auto pFunction = GetProcAddress(mDLib, aName.c_str());
			if(pFunction == NULL) {
				std::wcout << L"Win32DLib: Failed to get the function. (Error: " << GetLastError() << ")" << std::endl;
				return nullptr;
			}

			return RCast(void*)(pFunction);
		}
	} // namespace platform
} // namespace cube

#endif // _WIN32
