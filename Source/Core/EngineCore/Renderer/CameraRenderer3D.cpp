#include "CameraRenderer3D.h"

#include <gtc\matrix_transform.hpp>

namespace cube
{
	namespace core
	{
		CameraRenderer3D::CameraRenderer3D()
		{
			mProjectionMatrix = glm::perspectiveFov(glm::radians(60.0f), 1024.0f, 768.0f, 0.1f, 100.0f);
			mViewMatrix = glm::lookAt(
				glm::vec3(3, -4, 5),
				glm::vec3(0, 0, 0),
				glm::vec3(0, 1, 0)
			);
		}

		CameraRenderer3D::~CameraRenderer3D()
		{
		}
	}
}