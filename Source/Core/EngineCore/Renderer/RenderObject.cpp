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
			mRenderObject = CreateRenderObject();
		}
	} // namespace core
} // namespace cube
