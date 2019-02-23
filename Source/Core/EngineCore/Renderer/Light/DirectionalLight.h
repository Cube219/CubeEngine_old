#pragma once

#include "../../EngineCoreHeader.h"

#include "BaseLight.h"

namespace cube
{
	namespace core
	{
		class ENGINE_CORE_EXPORT DirectionalLight : public BaseLight
		{
		public:
			virtual ~DirectionalLight();

			static SPtr<DirectionalLight> Create();

			virtual SPtr<rt::RenderObject> CreateRenderObject() const override;

			SPtr<rt::DirectionalLight> GetRenderObject() const { return DPCast(rt::DirectionalLight)(mRenderObject); }

			void SetDirection(const Vector3& direction);

			Vector3 GetDirection() const { return mDirection; }

		private:
			DirectionalLight();

			Vector3 mDirection;
		};

		namespace rt
		{
			class DirectionalLight : public BaseLight
			{
			public:
				virtual ~DirectionalLight() {}

				Vector3 GetDirection() const { return mDirection; }

			private:
				friend class cube::core::DirectionalLight;

				DirectionalLight() {}

				Vector3 mDirection;
			};
		} // namespace rt
	} // namespace core
} // namespace cube
