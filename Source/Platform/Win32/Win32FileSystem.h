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

			uint64_t GetFileSizeImpl();

			void SetFilePointerImpl(uint64_t offset);
			void MoveFilePointerImpl(int64_t distance);

			void ReadImpl(void* pReadBuffer, uint64_t bufferSizeToRead, uint64_t& readBufferSize);
			void WriteImpl(void* pWriteBuffer, uint64_t bufferSize);

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

			static SPtr<File> OpenFileImpl(const String& path, FileAccessModeBits accessModeBits, bool createIfNotExist = false);

		private:
			static DWORD GetDwDesiredAccess(FileAccessModeBits accessMode);
		};
		FILE_SYSTEM_DEFINITION(Win32FileSystem)
	}
}

#endif // _WIN32
