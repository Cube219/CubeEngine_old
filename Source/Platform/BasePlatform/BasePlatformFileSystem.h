#pragma once

#include "Base\BaseTypes.h"

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

		class BasePlatformFile;

		class BasePlatformFileSystem
		{
		public:
			BasePlatformFileSystem(){ }
			virtual ~BasePlatformFileSystem(){ }

			virtual SPtr<BasePlatformFile> OpenFile(WString& path, FileAccessModeBits accessModeBits, bool createIfNotExist = false) = 0;
		};

		class BasePlatformFile
		{
		public:
			friend class BasePlatformFileSystem;

			BasePlatformFile() { }
			virtual ~BasePlatformFile() { }

			virtual void Read(void* pReadBuffer, uint64_t bufferSizeToRead, uint64_t& bufferSize) = 0;

			virtual void Write(void* pWriteBuffer, uint64_t bufferSize) = 0;
		};
	}
}