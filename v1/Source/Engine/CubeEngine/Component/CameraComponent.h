#pragma once

#include "../CubeEngineHeader.h"

#include "EngineCore/Component/Component.h"
#include "EngineCore/Renderer/CameraRenderer3D.h"

#include "Base/Matrix.h"

namespace cube
{
	class CUBE_ENGINE_EXPORT CameraComponent : public Component
	{
	public:
		static const String& GetName() { return mName; }

	private:
		static String mName;

	public:
		CameraComponent();
		virtual ~CameraComponent();

		void OnInit() override;
		void OnUpdate(float dt) override;
		void OnDestroy() override;
		void OnTransformChanged() override;

	private:
		Matrix mViewMatrix;

		SPtr<CameraRenderer3D> mCameraRenderer3D;
	};

	using HCameraComponent = Handler<CameraComponent>;
} // namespace cube
