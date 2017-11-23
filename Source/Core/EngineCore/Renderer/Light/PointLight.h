#pragma once

#include "../../EngineCoreHeader.h"

#include <glm.hpp>

#include "BaseLight.h"

namespace cube
{
	namespace core
	{
		class PointLight : public BaseLight
		{
		public:
			PointLight();
			virtual ~PointLight();

			void SetRange(float range) { mRange = range; }
			void SetmAttenuation(glm::vec3& att) { mAttenuation = att; }

			float GetRange() const { return mRange; }
			glm::vec3 GetmAttenuation() const { return mAttenuation; }

		private:
			float mRange;
			glm::vec3 mAttenuation; // TODO: ¹»±î?
		};
	}
}