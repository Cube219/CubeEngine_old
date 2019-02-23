#include "CameraRenderer3D.h"

#include "Base/MatrixUtility.h"
#include "Base/Math.h"

namespace cube
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

	SPtr<rt::RenderObject> CameraRenderer3D::CreateRenderObject() const
	{
		SPtr<rt::CameraRenderer3D> cameraRenderer3d_rt(new rt::CameraRenderer3D());
		cameraRenderer3d_rt->Initialize();

		return cameraRenderer3d_rt;
	}

	void CameraRenderer3D::SetViewMatrix(const Matrix& matrix)
	{
		mViewMatrix = matrix;
		QueueSyncTask([this]() {
			GetRenderObject()->SyncViewProjectionMatrix(mViewMatrix, mProjectionMatrix);
		});
	}

	void CameraRenderer3D::SetPosition(const Vector3& pos)
	{
		mPosition = pos;
		QueueSyncTask([this]() {
			GetRenderObject()->SyncPosition(mPosition);
		});
	}

	namespace rt
	{
		void CameraRenderer3D::SyncViewProjectionMatrix(const Matrix& view, const Matrix& projection)
		{
			mViewProjectionMatrix = view * projection;
		}

		void CameraRenderer3D::SyncPosition(const Vector3& position)
		{
			mPosition = position;
		}
	} // namespace rt
} // namespace cube
