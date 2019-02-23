#include "PointLight.h"

namespace cube
{
	namespace core
	{
		PointLight::PointLight()
		{
		}

		PointLight::~PointLight()
		{
		}

		SPtr<PointLight> PointLight::Create()
		{
			SPtr<PointLight> pointLight(new PointLight());
			pointLight->Initialize();

			return pointLight;
		}

		SPtr<rt::RenderObject> PointLight::CreateRenderObject() const
		{
			SPtr<rt::PointLight> pointLight_rt(new rt::PointLight());
			pointLight_rt->Initialize();

			return pointLight_rt;
		}
	} // namespace core
} // namespace cube
