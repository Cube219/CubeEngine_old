#pragma once

#include "EngineCoreHeader.h"

#include "LogWriter.h"

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
#ifdef _DEBUG
				if(mData == nullptr || mData->data == nullptr) {
					CUBE_LOG(LogType::Error, "Handler<{0}> does not have the data", typeid(T).name());
					return nullptr;
				}
#endif // _DEBUG
				return &*(mData->data);
			}
			T& operator*() const
			{
#ifdef _DEBUG
				if(mData == nullptr || mData->data == nullptr) {
					CUBE_LOG(LogType::Error, "Handler<{0}> does not have the data", typeid(T).name());
					return nullptr;
				}
#endif // _DEBUG
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
