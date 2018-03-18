#pragma once

#include "../DLib.h"

#include <Windows.h>

namespace cube
{
	namespace platform
	{
		class Win32DLib : public DLib
		{
		public:
			Win32DLib(const WString& path);
			virtual ~Win32DLib();
		};
	}
}
