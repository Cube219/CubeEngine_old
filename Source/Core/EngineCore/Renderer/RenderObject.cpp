#include "RenderObject.h"

namespace cube
{
	namespace core
	{
		SPtr<RenderObject> RenderObject::Create()
		{
			SPtr<RenderObject> ro(new RenderObject());
			ro->Initialize();

			return ro;
		}

		void RenderObject::Initialize()
		{
			mRenderObject_RT = CreateRenderObject_RT();
		}
	} // namespace core
} // namespace cube
