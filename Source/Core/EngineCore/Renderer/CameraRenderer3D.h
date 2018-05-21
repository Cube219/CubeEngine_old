#pragma once

#include "../EngineCoreHeader.h"

#include "RenderObject.h"
#include "Base/Matrix.h"

namespace cube
{
	namespace core
	{
		class ENGINE_CORE_EXPORT CameraRenderer3D : public RenderObject
		{
		public:
			static SPtr<CameraRenderer3D> Create();

		public:
			virtual ~CameraRenderer3D();

			virtual SPtr<RenderObject_RT> CreateRenderObject_RT() const override;
			SPtr<CameraRenderer3D_RT> GetRenderObject_RT() const { return DPCast(CameraRenderer3D_RT)(mRenderObject_RT); }

			void SetViewMatrix(const Matrix& matrix);
			void SetPosition(const Vector3& pos);

		private:
			CameraRenderer3D();

			Matrix mProjectionMatrix;
			Matrix mViewMatrix;
			Vector3 mPosition;
		};

		class CameraRenderer3D_RT : public RenderObject_RT
		{
		public:
			virtual ~CameraRenderer3D_RT(){ }

			void SyncViewProjectionMatrix(const Matrix& view, const Matrix& projection);
			void SyncPosition(const Vector3& position);

			Matrix GetViewProjectionMatrix() const { return mViewProjectionMatrix; }
			Vector3 GetPosition() const { return mPosition; }

		private:
			friend class CameraRenderer3D;

			CameraRenderer3D_RT(){ }

			Matrix mViewProjectionMatrix;
			Vector3 mPosition;
		};
	} // namespace core
} // namespace cube
