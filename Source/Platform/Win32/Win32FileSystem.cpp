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
		uint64_t Win32File::GetFileSizeImpl()
		{
			LARGE_INTEGER size_LI;
			BOOL res = GetFileSizeEx(mFileHandle, &size_LI);

			if(res == FALSE) {
				DWORD err = GetLastError();
				std::wcout << L"Win32File: Failed to get file size. / ErrorCode: " << err << std::endl;
				return 0;
			}

			return size_LI.QuadPart;
		}

		void Win32File::SetFilePointerImpl(uint64_t offset)
		{
			LARGE_INTEGER distance_LI;
			distance_LI.QuadPart = offset;

			BOOL res = SetFilePointerEx(mFileHandle, distance_LI, NULL, FILE_BEGIN);

			if(res == FALSE) {
				DWORD err = GetLastError();
				std::wcout << L"Win32File: Failed to set file pointer. / ErrorCode: " << err << std::endl;
			}
		}

		void Win32File::MoveFilePointerImpl(int64_t distance)
		{
			LARGE_INTEGER distance_LI;
			distance_LI.QuadPart = distance;

			BOOL res = SetFilePointerEx(mFileHandle, distance_LI, NULL, FILE_CURRENT);

			if(res == FALSE) {
				DWORD err = GetLastError();
				std::wcout << L"Win32File: Failed to move file pointer. / ErrorCode: " << err << std::endl;
			}
		}

		void Win32File::ReadImpl(void* pReadBuffer, uint64_t bufferSizeToRead, uint64_t& readBufferSize)
		{
			BOOL res = ReadFile(mFileHandle, pReadBuffer, (DWORD)bufferSizeToRead, (LPDWORD)&readBufferSize, NULL);

			if(res == FALSE) {
				DWORD err = GetLastError();
				std::wcout << L"Win32File: Failed to read the file. / ErrorCode: " << err << std::endl;
			}
		}

		void Win32File::WriteImpl(void* pWriteBuffer, uint64_t bufferSize)
		{
			DWORD writtenSize;
			BOOL res = WriteFile(mFileHandle, pWriteBuffer, (DWORD)bufferSize, &writtenSize, nullptr);

			if(res == FALSE) {
				DWORD err = GetLastError();
				std::wcout << L"Win32File: Failed to write the file. / ErrorCode: " << err << std::endl;
			}
		}

		Win32File::Win32File(HANDLE fileHandle) : 
			mFileHandle(fileHandle)
		{
		}

		Win32File::~Win32File()
		{
			CloseHandle(mFileHandle);
		}

		////////////////
		// FileSystem //
		////////////////

		SPtr<File> Win32FileSystem::OpenFileImpl(const String& path, FileAccessModeBits accessModeBits, bool createIfNotExist)
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

		DWORD Win32FileSystem::GetDwDesiredAccess(FileAccessModeBits accessMode)
		{
			DWORD d = 0;

			if(static_cast<int>(accessMode & FileAccessModeBits::Read) > 0)
				d |= GENERIC_READ;

			if(static_cast<int>(accessMode & FileAccessModeBits::Write) > 0)
				d |= GENERIC_WRITE;

			return d;
		}
	} // namespace platform
} // namespace cube

#endif // _WIN32
