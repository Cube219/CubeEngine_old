#pragma once

#include "../CubeEngineHeader.h"

#include "EngineCore/Component/Component.h"
#include "EngineCore/Renderer/CameraRenderer3D.h"

#include <glm.hpp>

namespace cube
{
	class CameraComponent : public core::Component
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

	private:
		glm::mat4 mViewMatrix;

		SPtr<core::CameraRenderer3D> mCameraRenderer3D;
	};
} // namespace cube
