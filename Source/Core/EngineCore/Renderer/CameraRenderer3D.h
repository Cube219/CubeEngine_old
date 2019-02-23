#pragma once

#include "../EngineCoreHeader.h"

#include "RenderObject.h"
#include "Base/Matrix.h"

namespace cube
{
	class ENGINE_CORE_EXPORT CameraRenderer3D : public RenderObject
	{
	public:
		static SPtr<CameraRenderer3D> Create();

	public:
		virtual ~CameraRenderer3D();

		virtual SPtr<rt::RenderObject> CreateRenderObject() const override;
		SPtr<rt::CameraRenderer3D> GetRenderObject() const { return DPCast(rt::CameraRenderer3D)(mRenderObject); }

		void SetViewMatrix(const Matrix& matrix);
		void SetPosition(const Vector3& pos);

	private:
		CameraRenderer3D();

		Matrix mProjectionMatrix;
		Matrix mViewMatrix;
		Vector3 mPosition;
	};

	namespace rt
	{
		class CameraRenderer3D : public rt::RenderObject
		{
		public:
			virtual ~CameraRenderer3D() {}

			void SyncViewProjectionMatrix(const Matrix& view, const Matrix& projection);
			void SyncPosition(const Vector3& position);

			Matrix GetViewProjectionMatrix() const { return mViewProjectionMatrix; }
			Vector3 GetPosition() const { return mPosition; }

		private:
			friend class cube::CameraRenderer3D;

			CameraRenderer3D() {}

			Matrix mViewProjectionMatrix;
			Vector3 mPosition;
		};
	} // namespace rt
} // namespace cube
