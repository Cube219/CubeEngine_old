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
			Win32DLib(const String2& path);
			virtual ~Win32DLib();
		};
	}
}

#endif // _WIN32
