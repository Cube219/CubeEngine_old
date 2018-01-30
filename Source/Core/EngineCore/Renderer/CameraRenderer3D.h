#pragma once

#include "../EngineCoreHeader.h"

#include <glm.hpp>

namespace cube
{
	namespace core
	{
		class CameraRenderer3D
		{
		public:
			CameraRenderer3D();
			~CameraRenderer3D();

			void RotateTemp(float dt);

			glm::mat4 GetViewProjectionMatrix() const { return mProjectionMatrix * mViewMatrix; };

		private:
			glm::mat4 mProjectionMatrix;
			glm::mat4 mViewMatrix;

			float mAngleTemp;
		};
	}
}