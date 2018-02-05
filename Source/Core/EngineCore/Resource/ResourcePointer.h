#pragma once

#ifdef ENGINE_CORE_EXPORTS
#define ENGINE_CORE_EXPORT __declspec(dllexport) 
#else // ENGINE_CORE_EXPORTS
#define ENGINE_CORE_EXPORT __declspec(dllimport) 
#endif // ENGINE_CORE_EXPORTS

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
			ResourcePointer()
			{
			}

			~ResourcePointer()
			{
				mRes->mRefCount--;
			}

			ResourcePointer(const ResourcePointer<T>& other)
			{
				mRes = other.mRes;
				mRes->mRefCount++;
			}
			ResourcePointer<T>& operator=(const ResourcePointer<T>& rhs)
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
				return mRes;
			}
			T& operator*() const
			{
				return *mRes;
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

			T* mRes;
		};

		template <typename T>
		using RPtr = ResourcePointer<T>;
	}
}