#include "Component.h"

#include "../GameObject.h"

namespace cube
{
	namespace core
	{
		String Component::mName = CUBE_T("Component");

		Component::Component()
		{
		}

		Component::~Component()
		{
		}

		void Component::Destroy()
		{
			mMyHandler.mData->data = nullptr;
		}
	} // namespace core
} // namespace cube