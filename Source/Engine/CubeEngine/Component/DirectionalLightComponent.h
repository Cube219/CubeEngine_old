#pragma once

#include "../CubeEngineHeader.h"

#include "EngineCore/Component/Component.h"
#include "EngineCore/Renderer/Light/DirectionalLight.h"

namespace cube
{
	class CUBE_ENGINE_EXPORT DirectionalLightComponent : public Component
	{
	public:
		static const String& GetName() { return mName; }

	private:
		static String mName;

	public:
		DirectionalLightComponent();
		virtual ~DirectionalLightComponent();

		void OnInit() override;
		void OnUpdate(float dt) override;
		void OnDestroy() override;
		void OnTransformChanged() override;

		void SetColor(const Vector4& color);

	private:
		HDirectionalLight mDirLight;
	};

	using HDirectionalLightComponent = Handler<DirectionalLightComponent>;
} // namespace cube
