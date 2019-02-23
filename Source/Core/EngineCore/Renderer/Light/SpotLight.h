#pragma once

#include "../../EngineCoreHeader.h"

#include "BaseLight.h"

namespace cube
{
	class SpotLight : public BaseLight
	{
	public:
		SpotLight();
		virtual ~SpotLight();

		void SetRange(float range) { mRange = range; }
		void SetDirection(Vector3& direction) { mDirection = direction; }
		void SetSpot(float spot) { mSpot = spot; }
		void SetAttenuation(Vector3& att) { mAttenuation = att; }

		float GetRange() const { return mRange; }
		Vector3 GetDirection() const { return mDirection; }
		float GetSpot() const { return mSpot; }
		Vector3 GetAttenuation() const { return mAttenuation; }

	private:
		float mRange;
		Vector3 mDirection;
		float mSpot;

		Vector3 mAttenuation;
	};
} // namespace cube
