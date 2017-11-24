#include "WinFileSystem.h"

#include <iostream>

namespace cube
{
	namespace platform
	{
		DWORD GetDwDesiredAccess(FileAccessModeBits accessMode)
		{
			DWORD d = 0;

			if(static_cast<int>(accessMode & FileAccessModeBits::Read) > 0)
				d |= GENERIC_READ;

			if(static_cast<int>(accessMode & FileAccessModeBits::Write) > 0)
				d |= GENERIC_WRITE;

			return d;
		}

		WinFileSystem::WinFileSystem(HINSTANCE instance)
		{
		}

		WinFileSystem::~WinFileSystem()
		{
		}

		SPtr<BasePlatformFile> WinFileSystem::OpenFile(WString& path, FileAccessModeBits accessModeBits, bool createIfNotExist)
		{
			DWORD desiredAccess = GetDwDesiredAccess(accessModeBits);

			auto file = CreateFile(path.c_str(), desiredAccess, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

			if(file != INVALID_HANDLE_VALUE) {
				return SPtr<WinFile>(new WinFile(file));
			}

			auto err = GetLastError();

			if(err == ERROR_FILE_NOT_FOUND && createIfNotExist == true) {
				file = CreateFile(path.c_str(), desiredAccess, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

				if(file != INVALID_HANDLE_VALUE)
					return SPtr<WinFile>(new WinFile(file));

				err = GetLastError();
			}

			std::wcout << L"WinFileSystem: Cannot open a file. (" << path << L") / ErrorCode: " << err << std::endl;
			return nullptr;
		}

		// ------------------------------
		//            WinFile
		// ------------------------------

		WinFile::WinFile(HANDLE fileHandle) : mFileHandle(fileHandle)
		{
		}

		WinFile::~WinFile()
		{
			CloseHandle(mFileHandle);
		}

		uint64_t WinFile::GetFileSize() const
		{
			LARGE_INTEGER size_LI;
			bool res = GetFileSizeEx(mFileHandle, &size_LI);

			if(res == FALSE) {
				auto err = GetLastError();
				std::wcout << L"WinFile: Cannot get file size. / ErrorCode: " << err << std::endl;
				return 0;
			}

			return size_LI.QuadPart;
		}

		void WinFile::SetFilePointer(uint64_t offset)
		{
			LARGE_INTEGER distance_LI;
			distance_LI.QuadPart = offset;

			bool res = SetFilePointerEx(mFileHandle, distance_LI, NULL, FILE_BEGIN);

			if(res == FALSE) {
				auto err = GetLastError();
				std::wcout << L"WinFile: Cannot set file pointer. / ErrorCode: " << err << std::endl;
			}
		}

		void WinFile::MoveFilePointer(int64_t distance)
		{
			LARGE_INTEGER distance_LI;
			distance_LI.QuadPart = distance;

			bool res = SetFilePointerEx(mFileHandle, distance_LI, NULL, FILE_CURRENT);

			if(res == FALSE) {
				auto err = GetLastError();
				std::wcout << L"WinFile: Cannot move file pointer. / ErrorCode: " << err << std::endl;
			}
		}

		void WinFile::Read(void* pReadBuffer, uint64_t bufferSizeToRead, uint64_t& readBufferSize)
		{
			auto res = ReadFile(mFileHandle, pReadBuffer, (DWORD)bufferSizeToRead, (LPDWORD)&readBufferSize, NULL);

			if(res == FALSE) {
				auto err = GetLastError();
				std::wcout << L"WinFile: Cannot read the file. / ErrorCode: " << err << std::endl;
			}
		}

		void WinFile::Write(void* pWriteBuffer, uint64_t bufferSize)
		{
			DWORD writtenSize;
			auto res = WriteFile(mFileHandle, pWriteBuffer, (DWORD)bufferSize, &writtenSize, nullptr);

			if(res == FALSE) {
				auto err = GetLastError();
				std::wcout << L"WinFile: Cannot write the file. / ErrorCode: " << err << std::endl;
			}
		}
	}
}