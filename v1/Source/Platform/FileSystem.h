#pragma once

#include "PlatformHeader.h"

#include "Base/Flags.h"

namespace cube
{
	namespace platform
	{
		enum class FileAccessModeFlag
		{
			Read = 1,
			Write = 2
		};
		using FileAccessModeFlags = Flags<FileAccessModeFlag>;
		FLAGS_OPERATOR(FileAccessModeFlag);

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

			static SPtr<File> OpenFile(StringRef path, FileAccessModeFlags accessModeFlags, bool createIfNotExist = false);
		};

#define FILE_SYSTEM_DEFINITION(Child)                                                      \
		inline SPtr<File> FileSystem::OpenFile                                             \
			(StringRef path, FileAccessModeFlags accessModeFlags, bool createIfNotExist) { \
			return Child::OpenFileImpl(path, accessModeFlags, createIfNotExist);           \
		}
	} // namespace platform
} // namespace cube
