#pragma once

#include "../../EngineCoreHeader.h"

#include "BaseLight.h"

namespace cube
{
	namespace core
	{
		class DirectionalLight : public BaseLight
		{
		public:
			DirectionalLight();
			virtual ~DirectionalLight();

			void SetDirection(Vector3& direction) { mDirection = direction; }

			Vector3 GetDirection() const { return mDirection; }

		private:
			Vector3 mDirection;
		};
	}
}