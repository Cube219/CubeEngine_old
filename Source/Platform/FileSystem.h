#pragma once

#include "Base/BaseTypes.h"

namespace cube
{
	namespace platform
	{
		enum class FileAccessModeBits
		{
			Read = 1,
			Write = 2
		};
		SET_ENUM_AS_FLAGS(FileAccessModeBits);

		class File
		{
		public:
			File();
			~File();

			uint64_t GetFileSize();

			void SetFilePointer(uint64_t offset);
			void MoveFilePointer(int64_t distance);

			void Read(void* pReadBuffer, uint64_t bufferSizeToRead, uint64_t readBufferSize);

			void Write(void* pWriteBuffer, uint64_t bufferSize);

		private:
			friend class FileSystem;
			
			struct Data;
			Data* mData;
		};

		class FileSystem
		{
		public:
			FileSystem();
			~FileSystem();

			SPtr<File> OpenFile(const WString& path, FileAccessModeBits accessModeBits, bool createIfNotExist = false);

		private:
			struct Data;
			Data* mData;
		};
	}
}
