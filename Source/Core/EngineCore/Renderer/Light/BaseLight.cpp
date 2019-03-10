#include "BaseLight.h"

namespace cube
{
	BaseLight::BaseLight()
	{
	}

	BaseLight::~BaseLight()
	{
	}

	UPtr<BaseLight> BaseLight::Create()
	{
		UPtr<BaseLight> baseLight(new BaseLight());
		baseLight->Initialize();

		return baseLight;
	}

	SPtr<rt::RenderObject> BaseLight::CreateRenderObject() const
	{
		SPtr<rt::BaseLight> baseLight_rt(new rt::BaseLight());
		baseLight_rt->Initialize();

		return baseLight_rt;
	}

	void BaseLight::SetColor(const Vector4& color)
	{
		mColor = color;
		SyncPrimaryData(mColor, GetRenderObject()->mColor);
	}

	void BaseLight::SetPosition(const Vector3& pos)
	{
		mPosition = pos;
		SyncPrimaryData(mPosition, GetRenderObject()->mPosition);
	}
} // namespace cube
