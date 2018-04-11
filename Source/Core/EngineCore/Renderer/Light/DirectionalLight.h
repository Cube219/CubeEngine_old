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
			DirectionalLight();
			virtual ~DirectionalLight();

			void SetDirection(const Vector3& direction) { mDirection = direction; }

			Vector3 GetDirection() const { return mDirection; }

		private:
			Vector3 mDirection;
		};
	} // namespace core
} // namespace cube
