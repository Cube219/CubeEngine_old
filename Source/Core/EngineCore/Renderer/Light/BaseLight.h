#pragma once

#include "../../EngineCoreHeader.h"

#include <glm.hpp>

namespace cube
{
	namespace core
	{
		class BaseLight
		{
		public:
			BaseLight();
			virtual ~BaseLight();

			void SetAmbient(glm::vec4& ambient) { mAmbient = ambient; }
			void SetDiffuse(glm::vec4& diffuse) { mDiffuse = diffuse; }
			void SetSpecular(glm::vec4& specular) { mSpecular = specular; }
			void SetPosition(glm::vec3& position) { mPosition = position; }

			glm::vec4 GetAmbient() const { return mAmbient; }
			glm::vec4 GetDiffuse() const { return mDiffuse; }
			glm::vec4 GetSpecular() const { return mSpecular; }
			glm::vec3 GetPosition() const { return mPosition; }

		protected:
			glm::vec4 mAmbient;
			glm::vec4 mDiffuse;
			glm::vec4 mSpecular;
			
			glm::vec3 mPosition;
		};
	}
}
