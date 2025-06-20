#pragma once

#include <iostream>

#include "BaseResource.h"

namespace cube
{
	class ResourceManager;
	class ResourceImporter;

	template <typename T>
	class ResourcePointer
	{
	public:
		ResourcePointer() : mRes(nullptr)
		{
		}

		~ResourcePointer()
		{
			if(mRes != nullptr)
				mRes->mRefCount--;
		}

		ResourcePointer(nullptr_t nul) : mRes(nullptr)
		{
		}

		ResourcePointer(const ResourcePointer& other)
		{
			mRes = other.mRes;
			if(mRes != nullptr)
				mRes->mRefCount++;
		}
		ResourcePointer& operator=(const ResourcePointer& rhs)
		{
			if(this == &rhs)
				return *this;

			if(mRes != nullptr)
				mRes->mRefCount--;

			mRes = rhs.mRes;
			if(mRes != nullptr)
				mRes->mRefCount++;

			return *this;
		}

		bool operator==(nullptr_t nul) const { return mRes == nullptr; }
		bool operator!=(nullptr_t nul) const { return mRes != nullptr; }

		template <typename T2>
		ResourcePointer<T2> Cast()
		{
			ResourcePointer<T2> resPtr((T2*)mRes);
			return resPtr;
		}

		void Release()
		{
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
		friend class BaseMeshGenerator;
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
} // namespace cube
