#pragma once

#include "..\EngineCoreHeader.h"

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

			glm::mat4 GetViewProjectionMatrix() const;

		private:
			glm::mat4 mProjectionMatrix;
			glm::mat4 mViewMatrix;
		};

		inline glm::mat4 CameraRenderer3D::GetViewProjectionMatrix() const
		{
			return mProjectionMatrix * mViewMatrix;
		}
	}
}