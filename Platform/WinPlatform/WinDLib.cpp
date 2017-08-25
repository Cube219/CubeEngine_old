#include "WinDLib.h"

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
				WriteErrorLog(L"Cannot load a DLib. (" + path + L")");
			}
		}

		WinDLib::~WinDLib()
		{
			if(!mDLib)
				return;

			BOOL r = FreeLibrary(mDLib);
			if(r == 0) {
				WriteErrorLog(L"Failed to unload the DLib.");
			}
		}

		void* WinDLib::GetFunction(String name)
		{
			if(!mDLib)
				return nullptr;

			auto pFunction = GetProcAddress(mDLib, name.c_str());
			if(pFunction == NULL) {
				WriteErrorLog(L"Failed to get the function.");
				auto err = GetLastError();
				WriteErrorLog(std::to_wstring(err));
				return nullptr;
			}

			return RCast(void*)(pFunction);
		}
	}
}