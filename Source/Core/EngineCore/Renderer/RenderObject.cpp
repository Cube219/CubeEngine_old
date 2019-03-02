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

		RenderingThread::QueueTask([this]() {
			mRenderObject->Initialize();
		});
	}

	void RenderObject::Destroy()
	{
		RenderingThread::QueueTask([ro = mRenderObject]() {
			ro->Destroy();
		});

		mRenderObject = nullptr;
	}
} // namespace cube
