#ifdef _WIN32

#include "Win32FileSystem.h"

#include <iostream>

#include "../PlatformString.h"

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

		uint64_t File::GetFileSize()
		{
			LARGE_INTEGER size_LI;
			BOOL res = GetFileSizeEx(mData->fileHandle, &size_LI);

			if(res == FALSE) {
				DWORD err = GetLastError();
				std::wcout << L"Win32File: Cannot get file size. / ErrorCode: " << err << std::endl;
				return 0;
			}

			return size_LI.QuadPart;
		}

		void File::SetFilePointer(uint64_t offset)
		{
			LARGE_INTEGER distance_LI;
			distance_LI.QuadPart = offset;

			BOOL res = SetFilePointerEx(mData->fileHandle, distance_LI, NULL, FILE_BEGIN);

			if(res == FALSE) {
				DWORD err = GetLastError();
				std::wcout << L"Win32File: Cannot set file pointer. / ErrorCode: " << err << std::endl;
			}
		}

		void File::MoveFilePointer(int64_t distance)
		{
			LARGE_INTEGER distance_LI;
			distance_LI.QuadPart = distance;

			BOOL res = SetFilePointerEx(mData->fileHandle, distance_LI, NULL, FILE_CURRENT);

			if(res == FALSE) {
				DWORD err = GetLastError();
				std::wcout << L"Win32File: Cannot move file pointer. / ErrorCode: " << err << std::endl;
			}
		}

		void File::Read(void* pReadBuffer, uint64_t bufferSizeToRead, uint64_t readBufferSize)
		{
			BOOL res = ReadFile(mData->fileHandle, pReadBuffer, (DWORD)bufferSizeToRead, (LPDWORD)&readBufferSize, NULL);

			if(res == FALSE) {
				DWORD err = GetLastError();
				std::wcout << L"Win32File: Cannot read the file. / ErrorCode: " << err << std::endl;
			}
		}

		void File::Write(void* pWriteBuffer, uint64_t bufferSize)
		{
			DWORD writtenSize;
			BOOL res = WriteFile(mData->fileHandle, pWriteBuffer, (DWORD)bufferSize, &writtenSize, nullptr);

			if(res == FALSE) {
				DWORD err = GetLastError();
				std::wcout << L"Win32File: Cannot write the file. / ErrorCode: " << err << std::endl;
			}
		}

		Win32File::Win32File(HANDLE fileHandle)
		{
			mData = new File::Data();

			mData->fileHandle = fileHandle;
		}

		Win32File::~Win32File()
		{
			CloseHandle(mData->fileHandle);

			delete mData;
		}

		////////////////
		// FileSystem //
		////////////////
		struct FileSystem::Data
		{
		};

		DWORD GetDwDesiredAccess(FileAccessModeBits accessMode)
		{
			DWORD d = 0;

			if(static_cast<int>(accessMode & FileAccessModeBits::Read) > 0)
				d |= GENERIC_READ;

			if(static_cast<int>(accessMode & FileAccessModeBits::Write) > 0)
				d |= GENERIC_WRITE;

			return d;
		}

		SPtr<File> FileSystem::OpenFile(const String2& path, FileAccessModeBits accessModeBits, bool createIfNotExist)
		{
			DWORD desiredAccess = GetDwDesiredAccess(accessModeBits);
			PString pPath = ToPString(path);

			HANDLE file = CreateFile(pPath.c_str(), desiredAccess, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

			if(file != INVALID_HANDLE_VALUE) {
				return std::make_shared<Win32File>(file);
			}

			DWORD err = GetLastError();

			if(err == ERROR_FILE_NOT_FOUND && createIfNotExist == true) {
				file = CreateFile(pPath.c_str(), desiredAccess, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

				if(file != INVALID_HANDLE_VALUE)
					return std::make_shared<Win32File>(file);

				err = GetLastError();
			}

			std::wcout << L"Win32FileSystem: Cannot open a file. (" << pPath << L") / ErrorCode: " << err << std::endl;
			return nullptr;
		}

		Win32FileSystem::Win32FileSystem()
		{
			mData = new FileSystem::Data();
		}

		Win32FileSystem::~Win32FileSystem()
		{
			delete mData;
		}
	}
}

#endif // _WIN32
