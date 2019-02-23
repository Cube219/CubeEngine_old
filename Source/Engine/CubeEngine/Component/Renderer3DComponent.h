#pragma once

#include "../CubeEngineHeader.h"

#include "EngineCore/Component/Component.h"

namespace cube
{
	class CUBE_ENGINE_EXPORT Renderer3DComponent : public Component
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

		void SetMesh(RPtr<Mesh>& mesh);
		void SetMaterialInstance(HMaterialInstance& materialIns, uint32_t index);

	private:
		SPtr<Renderer3D> mRenderer3D;

		RendererManager& mRendererManager;
	};

	using Renderer3DComponentData = BasicHandlerData<Renderer3DComponent>;
	using HRenderer3DComponent = BasicHandler<Renderer3DComponent>;
} // namespace cube
