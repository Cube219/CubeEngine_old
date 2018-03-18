#pragma once

#include "WinPlatformHeader.h"

#include "BasePlatform\BasePlatformDLib.h"

namespace cube
{
	namespace platform
	{
		class WinDLib : public BasePlatformDLib
		{
		public:
			WinDLib(WString path);
			virtual ~WinDLib();

			void* GetFunction(String name) override;

		private:
			HMODULE mDLib;
		};
	}
}