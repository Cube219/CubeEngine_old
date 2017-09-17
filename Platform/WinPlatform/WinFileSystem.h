#pragma once

#include "WinPlatformHeader.h"

#include "BasePlatform\BasePlatformFileSystem.h"

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

			SPtr<BasePlatformFile> OpenFile(WString& path, FileAccessModeBits accessModeBits, bool createIfNotExist = false) override;

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

			void Read(void* pReadBuffer, uint64_t bufferSizeToRead, uint64_t& bufferSize) override;

			void Write(void* pWriteBuffer, uint64_t bufferSize) override;

		private:
			WinFile(HANDLE fileHandle);

			HANDLE mFileHandle;
		};
	}
}