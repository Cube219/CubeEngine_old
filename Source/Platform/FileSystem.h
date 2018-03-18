#pragma once

#include "PlatformHeader.h"

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

		class PLATFORM_EXPORT File
		{
		public:
			File(){ }
			virtual ~File(){ }

			uint64_t GetFileSize();

			void SetFilePointer(uint64_t offset);
			void MoveFilePointer(int64_t distance);

			void Read(void* pReadBuffer, uint64_t bufferSizeToRead, uint64_t readBufferSize);

			void Write(void* pWriteBuffer, uint64_t bufferSize);

		protected:
			friend class FileSystem;
			
			struct Data;
			Data* mData;
		};

		class PLATFORM_EXPORT FileSystem
		{
		public:
			FileSystem(){ }
			virtual ~FileSystem(){ }

			SPtr<File> OpenFile(const WString& path, FileAccessModeBits accessModeBits, bool createIfNotExist = false);

		protected:
			struct Data;
			Data* mData;
		};
	}
}
