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
			Vector3 GetPosition() const { return mPosition; }

			void SetViewMatrix(const Matrix& matrix);
			void SetPosition(const Vector3& pos) { mPosition = pos; }

		private:
			Matrix mProjectionMatrix;
			Matrix mViewMatrix;
			Vector3 mPosition;
		};
	} // namespace core
} // namespace cube
