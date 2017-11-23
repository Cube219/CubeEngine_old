#pragma once

#include "../../EngineCoreHeader.h"

#include <glm.hpp>

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

			void SetDirection(glm::vec3& direction) { mDirection = direction; }

			glm::vec3 GetDirection() const { return mDirection; }

		private:
			glm::vec3 mDirection;
		};
	}
}