#pragma once

#include "../EngineCoreHeader.h"

#include <glm.hpp>

namespace cube
{
	namespace core
	{
		class ENGINE_CORE_EXPORT CameraRenderer3D
		{
		public:
			CameraRenderer3D();
			~CameraRenderer3D();

			glm::mat4 GetViewProjectionMatrix() const { return mProjectionMatrix * mViewMatrix; };

			void SetViewMatrix(const glm::mat4& matrix);

		private:
			glm::mat4 mProjectionMatrix;
			glm::mat4 mViewMatrix;

			float mAngleTemp;
		};
	}
}
