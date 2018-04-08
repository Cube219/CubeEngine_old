#pragma once

#include "../EngineCoreHeader.h"

#include "Base/Matrix.h"

namespace cube
{
	namespace core
	{
		class ENGINE_CORE_EXPORT CameraRenderer3D
		{
		public:
			CameraRenderer3D();
			~CameraRenderer3D();

			Matrix GetViewProjectionMatrix() const { return mViewMatrix * mProjectionMatrix; };

			void SetViewMatrix(const Matrix& matrix);

		private:
			Matrix mProjectionMatrix;
			Matrix mViewMatrix;
		};
	} // namespace core
} // namespace cube
