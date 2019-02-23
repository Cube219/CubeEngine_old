#include "RenderObject.h"

namespace cube
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
} // namespace cube
