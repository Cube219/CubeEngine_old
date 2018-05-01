#pragma once

#include "../CubeEngineHeader.h"

#include "EngineCore/Component/Component.h"
#include "EngineCore/Renderer/Light/PointLight.h"

namespace cube
{
	class CUBE_ENGINE_EXPORT PointLightComponent : public core::Component
	{
	public:
		static const String& GetName() { return mName; }

	private:
		static String mName;

	public:
		PointLightComponent();
		virtual ~PointLightComponent();

		void OnInit() override;
		void OnUpdate(float dt) override;
		void OnDestroy() override;
		void OnTransformChanged() override;

		void SetColor(const Vector4& color);

	private:
		SPtr<core::PointLight> mPointLight;
	};

	using PointLightComponentData = core::BasicHandlerData<PointLightComponent>;
	using HPointLightComponent = core::BasicHandler<PointLightComponent>;
} // namespace cube
