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

			virtual SPtr<rt::RenderObject> CreateRenderObject() const override;

			SPtr<rt::PointLight> GetRenderObject() const { return DPCast(rt::PointLight)(mRenderObject); }

		private:
			PointLight();
		};

		namespace rt
		{
			class PointLight : public BaseLight
			{
			public:
				virtual ~PointLight() {}

			private:
				friend class cube::core::PointLight;

				PointLight() {}
			};
		} // namespace rt
	} // namespace core
} // namespace cube
