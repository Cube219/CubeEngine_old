#include "WinDLib.h"

#include <iostream>

namespace cube
{
	namespace platform
	{
		WinDLib::WinDLib(WString path) :
			mDLib(0)
		{
			path += L".dll";

			mDLib = LoadLibrary(path.c_str());

			if(mDLib == NULL) {
				std::wcout << L"WinDLib: Cannot load a DLib. (" << path << L")" << std::endl;
			}
		}

		WinDLib::~WinDLib()
		{
			if(!mDLib)
				return;

			BOOL r = FreeLibrary(mDLib);
			if(r == 0) {
				std::wcout << L"WinDLib: Failed to unload the DLib." << std::endl;
			}
		}

		void* WinDLib::GetFunction(String name)
		{
			if(!mDLib)
				return nullptr;

			auto pFunction = GetProcAddress(mDLib, name.c_str());
			if(pFunction == NULL) {
				std::wcout << L"WinDLib: Failed to get the function." << std::endl;
				auto err = GetLastError();
				std::wcout << err << std::endl;
				return nullptr;
			}

			return RCast(void*)(pFunction);
		}
	}
}