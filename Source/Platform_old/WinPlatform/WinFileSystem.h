#pragma once

#include "WinPlatformHeader.h"

#include "BasePlatform/BasePlatformFileSystem.h"

namespace cube
{
	namespace platform
	{
		DWORD GetDwDesiredAccess(FileAccessModeBits accessMode);

		class WinFileSystem : public BasePlatformFileSystem
		{
		public:
			WinFileSystem(HINSTANCE instance);
			virtual ~WinFileSystem();

			SPtr<BasePlatformFile> OpenFile(WString& path, FileAccessModeBits accessModeBits, bool createIfNotExist = false) final override;

		private:
			HINSTANCE mInstance;
		};

		// ------------------------------
		//            WinFile
		// ------------------------------

		class WinFile : public BasePlatformFile
		{
		public:
			friend class WinFileSystem;

			WinFile() = delete;
			virtual ~WinFile();

			uint64_t GetFileSize() const final override;

			void SetFilePointer(uint64_t offset) final override;
			void MoveFilePointer(int64_t distance) final override;

			void Read(void* pReadBuffer, uint64_t bufferSizeToRead, uint64_t& readBufferSize) final override;

			void Write(void* pWriteBuffer, uint64_t bufferSize) final override;

		private:
			WinFile(HANDLE fileHandle);

			HANDLE mFileHandle;
		};
	}
}