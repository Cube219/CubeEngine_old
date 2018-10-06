#pragma once

#ifdef _WIN32

#include "../PlatformHeader.h"

#include <Windows.h>

#include "../DLib.h"

namespace cube
{
	namespace platform
	{
		class PLATFORM_EXPORT Win32DLib : public DLib
		{
		public:
			Win32DLib(const String& path);
			virtual ~Win32DLib();

			void* GetFunctionImpl(const String& name);

		private:
			HMODULE mDLib;
		};
		DLIB_DEFINITION(Win32DLib);
	} // namespace platform
} // namespace cube

#endif // _WIN32
