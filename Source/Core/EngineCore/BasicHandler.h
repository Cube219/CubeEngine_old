#pragma once

#include "EngineCoreHeader.h"

#include "Assertion.h"

namespace cube
{
	namespace core
	{
		template <typename T>
		struct BasicHandlerData
		{
			SPtr<T> data;
		};

		template <typename T>
		class BasicHandler
		{
		public:
			BasicHandler() : mData(nullptr) {}
			
			BasicHandler(SPtr<BasicHandlerData<T>>& data): mData(data) {}

			T* operator->() const
			{
				CHECK(mData != nullptr && mData->data != nullptr, "Handler \"{0}\" does not have the data", typeid(T).name());
				return &*(mData->data);
			}
			T& operator*() const
			{
				CHECK(mData != nullptr && mData->data != nullptr, "Handler \"{0}\" does not have the data", typeid(T).name());
				return *(mData->data);
			}

			bool IsDestroyed() const
			{
				return mData->data == nullptr;
			}

			template <typename T2>
			BasicHandler<T2> Cast() const
			{
				// Try to force casting using pointer
				// Because BasicHandlerData should exist only one
				// This code violate strict aliasing rule, but I think it is okay...
				SPtr<BasicHandlerData<T2>> newDataPtr = *(SPtr<BasicHandlerData<T2>>*)(&mData);
				return BasicHandler<T2>(newDataPtr);
			}

		protected:
			friend T;
			SPtr<BasicHandlerData<T>> mData;
		};
	} // namespace core
} // namespace cube
