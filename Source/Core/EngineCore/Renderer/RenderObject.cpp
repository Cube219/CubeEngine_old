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
		SPtr<rt::RenderObject> ro = mRenderObject;

		RenderingThread::QueueTask([ro]() {
			ro->Destroy();
		});

		mRenderObject = nullptr;
	}
} // namespace cube
