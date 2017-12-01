#pragma once

#include "../EngineCoreHeader.h"

#include "../Thread/MutexLock.h"

namespace cube
{
	namespace core
	{
		class ResourceRawData
		{
		public:
			ResourceRawData(uint64_t dataSize) : 
				mSize(dataSize)
			{
				mRawData = malloc(dataSize);
			}
			~ResourceRawData()
			{
				free(mRawData);
			}

			const void* GetRawData() const { return mRawData; }
			const uint64_t GetSize() const { return mSize; }

		private:
			friend class ResourceManager;
			friend class BaseResource;

			void* mRawData;
			uint64_t mSize;

			Atomic<uint32_t> mRefCount;
		};

		class ENGINE_CORE_EXPORT BaseResource
		{
		public:
			virtual ~BaseResource()
			{
				mRawData->mRefCount--;
			}

			// Prohibit creating
			BaseResource() = delete;

			// Prohibit copying
			BaseResource(const BaseResource& other) = delete;
			BaseResource& operator=(const BaseResource& rhs) = delete;

			template <typename T>
			SPtr<T> CastResource()
			{
				SPtr<T> newRes(new T(this->mRawData));

				return newRes;
			}

		protected:
			friend class ResourceManager;

			// Prohibit creating except friend classes
			BaseResource(ResourceRawData* rawData) : 
				mRawData(rawData)
			{
				mRawData->mRefCount++;
			}

			ResourceRawData* mRawData;
		};
	}
}