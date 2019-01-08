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

		//////////
		// File //
		//////////

		class PLATFORM_EXPORT File
		{
		public:
			File(){ }
			virtual ~File(){ }

			uint64_t GetFileSize();

			void SetFilePointer(Uint64 offset);
			void MoveFilePointer(Int64 distance);

			void Read(void* pReadBuffer, Uint64 bufferSizeToRead, Uint64& readBufferSize);

			void Write(void* pWriteBuffer, Uint64 bufferSize);

		protected:
			friend class FileSystem;
		};

#define FILE_DEFINITION(Child)                                                            \
		inline Uint64 File::GetFileSize() {                                               \
			return RCast(Child*)(this)->GetFileSizeImpl();                                \
		}                                                                                 \
		                                                                                  \
		inline void File::SetFilePointer(Uint64 offset) {                                 \
			RCast(Child*)(this)->SetFilePointerImpl(offset);                              \
		}                                                                                 \
		inline void File::MoveFilePointer(Int64 distance) {                               \
			RCast(Child*)(this)->MoveFilePointerImpl(distance);                           \
		}                                                                                 \
		                                                                                  \
		inline void File::Read                                                            \
			(void* pReadBuffer, Uint64 bufferSizeToRead, Uint64& readBufferSize) {        \
			RCast(Child*)(this)->ReadImpl(pReadBuffer, bufferSizeToRead, readBufferSize); \
		}                                                                                 \
                                                                                          \
		inline void File::Write(void* pWriteBuffer, Uint64 bufferSize) {                  \
			RCast(Child*)(this)->WriteImpl(pWriteBuffer, bufferSize);                     \
		}

		////////////////
		// FileSystem //
		////////////////

		class PLATFORM_EXPORT FileSystem
		{
		public:
			FileSystem() = delete;
			~FileSystem() = delete;

			static SPtr<File> OpenFile(StringRef path, FileAccessModeBits accessModeBits, bool createIfNotExist = false);
		};

#define FILE_SYSTEM_DEFINITION(Child)                                                        \
		inline SPtr<File> FileSystem::OpenFile                                               \
			(StringRef path, FileAccessModeBits accessModeBits, bool createIfNotExist) { \
			return Child::OpenFileImpl(path, accessModeBits, createIfNotExist);              \
		}
	} // namespace platform
} // namespace cube
