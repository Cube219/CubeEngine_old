#pragma once

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
			Win32DLib(const WString& path);
			virtual ~Win32DLib();
		};
	}
}
