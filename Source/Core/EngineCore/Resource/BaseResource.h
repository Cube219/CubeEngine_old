#pragma once

#include "../EngineCoreHeader.h"

#include "../Thread/MutexLock.h"

namespace cube
{
	namespace core
	{
		template <typename T>
		class ENGINE_CORE_EXPORT ResourcePointer
		{
		public:
			ResourcePointer()
			{
				rawPtr->mRawData->mRefCount++;
			}
			~ResourcePointer()
			{
				rawPtr->mRawData->mRefCount--;
			}

			static ResourcePointer<T> CastFromBase(ResourcePointer<BaseResource>& base);

		private:
			friend class ResourceManager;

			T* rawPtr;
		};

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

		private:
			friend class ResourceManager;
			friend class ResourcePointer<BaseResource>;

			void* mRawData;
			uint64_t mSize;

			Atomic<uint32_t> mRefCount;
		};

		class ENGINE_CORE_EXPORT BaseResource
		{
		public:
			virtual ~BaseResource() { }

			// Prohibit creating
			BaseResource() = delete;

			// Prohibit copying
			BaseResource(const BaseResource& other) = delete;
			BaseResource& operator=(const BaseResource& rhs) = delete;

		protected:
			friend class ResourceManager;
			friend class ResourcePointer<BaseResource>;

			// Prohibit creating except friend classes
			BaseResource(ResourceRawData* rawData) : 
				mRawData(rawData)
			{ }

			ResourceRawData* mRawData;
		};
	}
}