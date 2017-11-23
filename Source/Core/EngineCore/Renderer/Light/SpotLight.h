#pragma once

#include "../../EngineCoreHeader.h"

#include <glm.hpp>

#include "BaseLight.h"

namespace cube
{
	namespace core
	{
		class SpotLight : public BaseLight
		{
		public:
			SpotLight();
			virtual ~SpotLight();

			void SetRange(float range) { mRange = range; }
			void SetDirection(glm::vec3& direction) { mDirection = direction; }
			void SetSpot(float spot) { mSpot = spot; }
			void SetAttenuation(glm::vec3& att) { mAttenuation = att; }

			float GetRange() const { return mRange; }
			glm::vec3 GetDirection() const { return mDirection; }
			float GetSpot() const { return mSpot; }
			glm::vec3 GetAttenuation() const { return mAttenuation; }

		private:
			float mRange;
			glm::vec3 mDirection;
			float mSpot;

			glm::vec3 mAttenuation;
		};
	}
}