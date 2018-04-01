#pragma once

#include "EngineCoreHeader.h"

namespace cube
{
	namespace core
	{
		template <typename T>
		struct BasicHandlerData
		{
			UPtr<T> data;
		};

		template <typename T>
		class ENGINE_CORE_EXPORT BasicHandler
		{
		public:
			BasicHandler() : mData(nullptr) {}
			
			BasicHandler(SPtr<BasicHandlerData<T>>& data): mData(data) {}

			T* operator->() const
			{
				return &*(mData->data);
			}
			T& operator*() const
			{
				return *(mData->data);
			}

			bool IsDestroyed() const
			{
				return mData->data != nullptr;
			}

		protected:
			SPtr<BasicHandlerData<T>> mData;
		};
	} // namespace core
} // namespace cube
