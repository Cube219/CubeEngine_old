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

		SPtr<RenderObject_RT> PointLight::CreateRenderObject_RT() const
		{
			SPtr<PointLight_RT> pointLight_rt(new PointLight_RT());
			pointLight_rt->Initialize();


			return pointLight_rt;
		}
	} // namespace core
} // namespace cube
