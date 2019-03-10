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

		class PLATFORM_EXPORT Win32File : public File
		{
		public:
			Win32File(HANDLE fileHandle);
			virtual ~Win32File();

			Uint64 GetFileSizeImpl();

			void SetFilePointerImpl(Uint64 offset);
			void MoveFilePointerImpl(Int64 distance);

			void ReadImpl(void* pReadBuffer, Uint64 bufferSizeToRead, Uint64& readBufferSize);
			void WriteImpl(void* pWriteBuffer, Uint64 bufferSize);

		private:
			HANDLE mFileHandle;
		};
		FILE_DEFINITION(Win32File)

		////////////////
		// FileSystem //
		////////////////

		class Win32FileSystem : public FileSystem
		{
		public:
			Win32FileSystem() = delete;
			~Win32FileSystem() = delete;

			static SPtr<File> OpenFileImpl(StringRef path, FileAccessModeFlags accessModeFlags, bool createIfNotExist = false);

		private:
			static DWORD GetDwDesiredAccess(FileAccessModeFlags accessModeFlags);
		};
		FILE_SYSTEM_DEFINITION(Win32FileSystem)
	}
}

#endif // _WIN32
