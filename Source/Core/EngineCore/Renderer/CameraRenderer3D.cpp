#include "CameraRenderer3D.h"

#include "Base/Math.h"

#include <gtc/matrix_transform.hpp>

namespace cube
{
	namespace core
	{
		CameraRenderer3D::CameraRenderer3D()
		{
			mProjectionMatrix = glm::perspectiveFov(glm::radians(60.0f), 1024.0f, 768.0f, 0.1f, 100.0f);
			mViewMatrix = glm::lookAt(
				glm::vec3(5, -5, 5), // Flip y, z
				glm::vec3(0, 0, 0),
				glm::vec3(0, 1, 0)
			);

			mAngleTemp = 0.0f;
		}

		CameraRenderer3D::~CameraRenderer3D()
		{
		}

		void CameraRenderer3D::RotateTemp(float dt)
		{
			mAngleTemp += 60.0f * dt;

			float angleRad = Math::Deg2Rad(mAngleTemp);

			float x = Math::Cos(angleRad) * 10;
			float z = Math::Sin(angleRad) * 10;

			mViewMatrix = glm::lookAt(
				glm::vec3(x, -5, z), // Flip y, z
				glm::vec3(0, 0, 0),
				glm::vec3(0, 1, 0)
			);
		}
	}
}