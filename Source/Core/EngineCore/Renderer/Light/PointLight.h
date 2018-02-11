#pragma once

#include "../../EngineCoreHeader.h"

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
			void SetmAttenuation(Vector3& att) { mAttenuation = att; }

			float GetRange() const { return mRange; }
			Vector3 GetmAttenuation() const { return mAttenuation; }

		private:
			float mRange;
			Vector3 mAttenuation; // TODO: ¹»±î?
		};
	}
}