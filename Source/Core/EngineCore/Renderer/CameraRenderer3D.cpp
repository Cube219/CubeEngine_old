#include "CameraRenderer3D.h"

#include "Base/MatrixUtility.h"
#include "Base/Math.h"

namespace cube
{
	namespace core
	{
		CameraRenderer3D::CameraRenderer3D()
		{
			mProjectionMatrix = MatrixUtility::GetPerspectiveFovWithReverseY(Math::Deg2Rad(60.0f), 1024.0f / 768.0f, 0.1f, 100.0f);
			mViewMatrix = MatrixUtility::GetLookAt(
				Vector3(5, -5, 5),
				Vector3(0, 0, 0),
				Vector3(0, 1, 0)
			);
		}

		SPtr<CameraRenderer3D> CameraRenderer3D::Create()
		{
			SPtr<CameraRenderer3D> cameraRenderer3d(new CameraRenderer3D());
			cameraRenderer3d->Initialize();

			return cameraRenderer3d;
		}

		CameraRenderer3D::~CameraRenderer3D()
		{
		}

		SPtr<RenderObject_RT> CameraRenderer3D::CreateRenderObject_RT() const
		{
			SPtr<CameraRenderer3D_RT> cameraRenderer3d_rt(new CameraRenderer3D_RT());
			cameraRenderer3d_rt->Initialize();

			return cameraRenderer3d_rt;
		}

		void CameraRenderer3D::SetViewMatrix(const Matrix& matrix)
		{
			mViewMatrix = matrix;
			QueueSyncTask([this]() {
				GetRenderObject_RT()->SyncViewProjectionMatrix(mViewMatrix, mProjectionMatrix);
			});
		}

		void CameraRenderer3D::SetPosition(const Vector3& pos)
		{
			mPosition = pos;
			QueueSyncTask([this]() {
				GetRenderObject_RT()->SyncPosition(mPosition);
			});
		}

		void CameraRenderer3D_RT::SyncViewProjectionMatrix(const Matrix& view, const Matrix& projection)
		{
			mViewProjectionMatrix = view * projection;
		}

		void CameraRenderer3D_RT::SyncPosition(const Vector3& position)
		{
			mPosition = position;
		}
	} // namespace core	
} // namespace cube
