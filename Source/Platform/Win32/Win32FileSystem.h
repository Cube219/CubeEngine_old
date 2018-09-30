#pragma once

#ifdef _WIN32

#include "../PlatformHeader.h"

#include <Windows.h>

#include "../FileSystem.h"

namespace cube
{
	namespace platform
	{
		//////////
		// File //
		//////////

		struct File::Data
		{
			HANDLE fileHandle;
		};

		class PLATFORM_EXPORT Win32File : public File
		{
		public:
			Win32File(HANDLE fileHandle);
			virtual ~Win32File();
		};

		////////////////
		// FileSystem //
		////////////////

		DWORD GetDwDesiredAccess(FileAccessModeBits accessMode);

		struct FileSystem::Data
		{
		};

		class Win32FileSystem : public FileSystem
		{
		public:
			Win32FileSystem();
			virtual ~Win32FileSystem();
		};
	}
}

#endif // _WIN32
