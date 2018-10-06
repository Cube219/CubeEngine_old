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

			void SetFilePointer(uint64_t offset);
			void MoveFilePointer(int64_t distance);

			void Read(void* pReadBuffer, uint64_t bufferSizeToRead, uint64_t& readBufferSize);

			void Write(void* pWriteBuffer, uint64_t bufferSize);

		protected:
			friend class FileSystem;
		};

#define FILE_DEFINITION(Child)                                                            \
		inline uint64_t File::GetFileSize() {                                             \
			return RCast(Child*)(this)->GetFileSizeImpl();                                \
		}                                                                                 \
		                                                                                  \
		inline void File::SetFilePointer(uint64_t offset) {                               \
			RCast(Child*)(this)->SetFilePointerImpl(offset);                              \
		}                                                                                 \
		inline void File::MoveFilePointer(int64_t distance) {                             \
			RCast(Child*)(this)->MoveFilePointerImpl(distance);                           \
		}                                                                                 \
		                                                                                  \
		inline void File::Read                                                            \
			(void* pReadBuffer, uint64_t bufferSizeToRead, uint64_t& readBufferSize) {    \
			RCast(Child*)(this)->ReadImpl(pReadBuffer, bufferSizeToRead, readBufferSize); \
		}                                                                                 \
                                                                                          \
		inline void File::Write(void* pWriteBuffer, uint64_t bufferSize) {                \
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

			static SPtr<File> OpenFile(const String& path, FileAccessModeBits accessModeBits, bool createIfNotExist = false);
		};

#define FILE_SYSTEM_DEFINITION(Child)                                                        \
		inline SPtr<File> FileSystem::OpenFile                                               \
			(const String& path, FileAccessModeBits accessModeBits, bool createIfNotExist) { \
			return Child::OpenFileImpl(path, accessModeBits, createIfNotExist);              \
		}
	} // namespace platform
} // namespace cube
