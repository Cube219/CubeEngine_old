#include "DirectionalLight.h"

namespace cube
{
	DirectionalLight::DirectionalLight()
	{
	}

	DirectionalLight::~DirectionalLight()
	{
	}

	UPtr<DirectionalLight> DirectionalLight::Create()
	{
		UPtr<DirectionalLight> dirLight(new DirectionalLight());
		dirLight->Initialize();

		return dirLight;
	}

	SPtr<rt::RenderObject> DirectionalLight::CreateRenderObject() const
	{
		SPtr<rt::DirectionalLight> dirLight_rt(new rt::DirectionalLight());
		dirLight_rt->Initialize();

		return dirLight_rt;
	}

	void DirectionalLight::SetDirection(const Vector3& direction)
	{
		mDirection = direction;
		SyncPrimaryData(mDirection, GetRenderObject()->mDirection);
	}
} // namespace cube
