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

			void SetColor(const Vector4& color) { mColor = color; }
			void SetDirection(const Vector3& direction) { mDirection = direction; }

			Vector4 GetColor() const { return mColor; }
			Vector3 GetDirection() const { return mDirection; }

		private:
			Vector4 mColor;
			Vector3 mDirection;
		};
	} // namespace core
} // namespace cube
