#pragma once

#include "../PlatformHeader.h"

#include <Windows.h>

#include "../FileSystem.h"

namespace cube
{
	namespace platform
	{
		class PLATFORM_EXPORT Win32File : public File
		{
		public:
			Win32File(HANDLE fileHandle);
			virtual ~Win32File();
		};

		DWORD GetDwDesiredAccess(FileAccessModeBits accessMode);

		class Win32FileSystem : public FileSystem
		{
		public:
			Win32FileSystem();
			virtual ~Win32FileSystem();
		};
	}
}
