#pragma once

#include "../CubeEngineHeader.h"

#include "EngineCore/Component/Component.h"
#include "EngineCore/Renderer/Light/PointLight.h"

namespace cube
{
	class CUBE_ENGINE_EXPORT PointLightComponent : public Component
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
		HPointLight mPointLight;
	};

	using HPointLightComponent = Handler<PointLightComponent>;
} // namespace cube
