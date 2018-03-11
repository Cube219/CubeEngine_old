#pragma once

#include <iostream>

#include "BaseResource.h"

namespace cube
{
	namespace core
	{
		class ResourceManager;
		class ResourceImporter;

		template <typename T>
		class ENGINE_CORE_EXPORT ResourcePointer
		{
		public:
			ResourcePointer() : 
				mRes(nullptr)
			{
			}

			~ResourcePointer()
			{
				if(mRes != nullptr)
					mRes->mRefCount--;
			}

			ResourcePointer(const ResourcePointer& other)
			{
				mRes = other.mRes;
				mRes->mRefCount++;
			}
			ResourcePointer& operator=(const ResourcePointer& rhs)
			{
				mRes = rhs.mRes;
				mRes->mRefCount++;

				return *this;
			}

			template <typename T2>
			ResourcePointer<T2> Cast()
			{
				ResourcePointer<T2> resPtr((T2*)mRes);
				return resPtr;
			}

			T* operator->() const
			{
				return (T*)mRes;
			}
			T& operator*() const
			{
				return *(T*)mRes;
			}

		private:
			friend class ResourceManager;
			friend class ResourceImporter;
			template <typename T2>
			friend class ResourcePointer;

			ResourcePointer(T* res)
			{
				mRes = res;
				mRes->mRefCount++;
			}

			Resource* mRes;
		};

		template <typename T>
		using RPtr = ResourcePointer<T>;
	}
}
