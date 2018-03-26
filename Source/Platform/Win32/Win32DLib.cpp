#include "Win32DLib.h"

#include <iostream>

#include "../PlatformString.h"

namespace cube
{
	namespace platform
	{
		struct DLib::Data
		{
			HMODULE dLib;
		};

		void* DLib::GetFunction(const String2& name)
		{
			if(!mData->dLib)
				return nullptr;

			std::string aName = ToASCIIString(name);
			auto pFunction = GetProcAddress(mData->dLib, aName.c_str());
			if(pFunction == NULL) {
				std::wcout << L"Win32DLib: Failed to get the function. (Error: " << GetLastError() << ")" << std::endl;
				return nullptr;
			}

			return RCast(void*)(pFunction);
		}

		Win32DLib::Win32DLib(const String2& path)
		{
			mData = new DLib::Data();

			PString pathWithExtension = ToPString(path) + L".dll";

			mData->dLib = LoadLibrary(pathWithExtension.c_str());

			if(mData->dLib == NULL) {
				std::wcout << L"Win32DLib: Cannot load a DLib. (" << pathWithExtension << L")" << std::endl;
			}
		}

		Win32DLib::~Win32DLib()
		{
			if(mData->dLib) {
				BOOL r = FreeLibrary(mData->dLib);
				if(r == 0) {
					std::wcout << L"Win32DLib: Failed to unload the DLib." << std::endl;
				}
			}

			delete mData;
		}
	}
}
