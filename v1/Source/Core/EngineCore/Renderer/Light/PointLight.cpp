#include "PointLight.h"

namespace cube
{
	PointLight::PointLight()
	{
	}

	PointLight::~PointLight()
	{
	}

	UPtr<PointLight> PointLight::Create()
	{
		UPtr<PointLight> pointLight(new PointLight());
		pointLight->Initialize();

		return pointLight;
	}

	SPtr<rt::RenderObject> PointLight::CreateRenderObject() const
	{
		SPtr<rt::PointLight> pointLight_rt(new rt::PointLight());
		pointLight_rt->Initialize();

		return pointLight_rt;
	}
} // namespace cube
