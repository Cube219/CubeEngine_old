#include "BaseLight.h"

namespace cube
{
	namespace core
	{
		BaseLight::BaseLight()
		{
		}

		BaseLight::~BaseLight()
		{
		}

		SPtr<BaseLight> BaseLight::Create()
		{
			SPtr<BaseLight> baseLight(new BaseLight());
			baseLight->Initialize();

			return baseLight;
		}

		SPtr<RenderObject_RT> BaseLight::CreateRenderObject_RT() const
		{
			SPtr<BaseLight_RT> baseLight_rt(new BaseLight_RT());
			baseLight_rt->Initialize();

			return baseLight_rt;
		}

		void BaseLight::SetColor(const Vector4& color)
		{
			mColor = color;
			SyncPrimaryData(mColor, GetRenderObject_RT()->mColor);
		}

		void BaseLight::SetPosition(const Vector3& pos)
		{
			mPosition = pos;
			SyncPrimaryData(mPosition, GetRenderObject_RT()->mPosition);
		}
	} // namespace core
} // namespace cube
