#include "BaseResource.h"

namespace cube
{
	namespace core
	{
		template <typename T>
		ResourcePointer<T> ResourcePointer<T>::CastFromBase(ResourcePointer<BaseResource>& base)
		{
			ResourcePointer<T> newPtr;
			newPtr.resPtr(new T(base.rawPtr.mRawData));

			return std::move(newPtr);
		}
	}
}