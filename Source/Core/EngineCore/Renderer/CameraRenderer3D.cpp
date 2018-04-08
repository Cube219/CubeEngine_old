#include "CameraRenderer3D.h"

#include <gtc/matrix_transform.hpp>
#include "Base/MatrixUtility.h"
#include "Base/Math.h"

namespace cube
{
	namespace core
	{
		CameraRenderer3D::CameraRenderer3D()
		{
			//mProjectionMatrix = glm::perspectiveFov(glm::radians(60.0f), 1024.0f, 768.0f, 0.1f, 100.0f);
			mProjectionMatrix = MatrixUtility::GetPerspectiveFov(Math::Deg2Rad(60.0f), 1024.0f / 768.0f, 0.1f, 100.0f);
			/*mViewMatrix = glm::lookAt(
				glm::vec3(5, -5, 5), // Flip y, z
				glm::vec3(0, 0, 0),
				glm::vec3(0, 1, 0)
			);*/
			mViewMatrix = MatrixUtility::GetLookAt(
				Vector3(5, -5, 5),
				Vector3(0, 0, 0),
				Vector3(0, 1, 0)
			);
		}

		CameraRenderer3D::~CameraRenderer3D()
		{
		}

		void CameraRenderer3D::SetViewMatrix(const Matrix& matrix)
		{
			mViewMatrix = matrix;
		}
	} // namespace core	
} // namespace cube
