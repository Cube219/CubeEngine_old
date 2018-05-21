#pragma once

#include "../../EngineCoreHeader.h"

#include "BaseLight.h"

namespace cube
{
	namespace core
	{
		class ENGINE_CORE_EXPORT PointLight : public BaseLight
		{
		public:
			virtual ~PointLight();

			static SPtr<PointLight> Create();

			virtual SPtr<RenderObject_RT> CreateRenderObject_RT() const override;

			SPtr<PointLight_RT> GetRenderObject_RT() const { return DPCast(PointLight_RT)(mRenderObject_RT); }

		private:
			PointLight();
		};

		class PointLight_RT : public BaseLight_RT
		{
		public:
			virtual ~PointLight_RT(){ }

		private:
			friend class PointLight;

			PointLight_RT(){ }
		};
	} // namespace core
} // namespace cube
