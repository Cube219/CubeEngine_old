#pragma once

#include "../FileSystem.h"

#include <Windows.h>

namespace cube
{
	namespace platform
	{
		class Win32File : public File
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
