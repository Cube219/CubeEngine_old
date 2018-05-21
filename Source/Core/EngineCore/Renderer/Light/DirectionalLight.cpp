#include "DirectionalLight.h"

namespace cube
{
	namespace core
	{
		DirectionalLight::DirectionalLight()
		{
		}

		DirectionalLight::~DirectionalLight()
		{
		}

		SPtr<DirectionalLight> DirectionalLight::Create()
		{
			SPtr<DirectionalLight> dirLight(new DirectionalLight());
			dirLight->Initialize();

			return dirLight;
		}

		SPtr<RenderObject_RT> DirectionalLight::CreateRenderObject_RT() const
		{
			SPtr<DirectionalLight_RT> dirLight_rt(new DirectionalLight_RT());
			dirLight_rt->Initialize();

			return dirLight_rt;
		}

		void DirectionalLight::SetDirection(const Vector3& direction)
		{
			mDirection = direction;
			SyncPrimaryData(mDirection, GetRenderObject_RT()->mDirection);
		}
	} // namespace core
} // namespace cube
