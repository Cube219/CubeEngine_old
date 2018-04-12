#pragma once

#include "../CubeEngineHeader.h"

#include "EngineCore/Component/Component.h"

namespace cube
{
	class CUBE_ENGINE_EXPORT Renderer3DComponent : public core::Component
	{
	public:
		static const String& GetName() { return mName; }

	private:
		static String mName;

	public:
		Renderer3DComponent();
		virtual ~Renderer3DComponent();

		void OnInit() override;
		void OnUpdate(float dt) override;
		void OnDestroy() override;
		void OnTransformChanged() override;

		void SetMesh(core::RPtr<core::Mesh>& mesh);
		void SetMaterialInstance(core::HMaterialInstance& materialIns);

	private:
		SPtr<core::Renderer3D> mRenderer3D;

		SPtr<core::RendererManager> mRendererManager;
	};

	using Renderer3DComponentData = core::BasicHandlerData<Renderer3DComponent>;
	using HRenderer3DComponent = core::BasicHandler<Renderer3DComponent>;
} // namespace cube
