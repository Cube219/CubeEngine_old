#pragma once

#include "../../EngineCoreHeader.h"

#include "Skybox.h"
#include "RenderAPI/Interface/Device.h"
#include "RenderAPI/Interface/GraphicsPipelineState.h"
#include "RenderAPI/Interface/ShaderParametersLayout.h"
#include "RenderAPI/Interface/ShaderParameters.h"
#include "RenderAPI/Interface/RenderPass.h"
#include "RenderAPI/Interface/CommandList.h"

namespace cube
{
	class SkyboxSystem
	{
	public:
		SkyboxSystem(RendererManager& manager) :
			mManager(manager)
		{}
		~SkyboxSystem() {}

		void Initialize();
		void ShutDown();

		SkyboxSystem(const SkyboxSystem& other) = delete;
		SkyboxSystem& operator=(const SkyboxSystem& rhs) = delete;

		HSkybox RegisterSkybox(UPtr<Skybox>&& skybox);
		UPtr<Skybox> UnregisterSkybox(HSkybox skybox);

		void DrawSkybox(render::Viewport& vp, render::Rect2D& scissor);
		SPtr<render::CommandList> GetCommandList() const { return mSkyboxCommandList; }

	private:
		void CreateSkyboxPipeline();
		void RewriteCommandList(render::Viewport& vp, render::Rect2D& scissor);

		RendererManager& mManager;

		SPtr<render::GraphicsPipelineState> mSkyboxPipeline;
		SPtr<render::ShaderParametersLayout> mSkyboxShaderParametersLayout;
		SPtr<render::ShaderParameters> mSkyboxShaderParameters;
		RPtr<Shader> mSkyboxVertexShader;
		RPtr<Shader> mSkyboxPixelShader;
		SPtr<render::CommandList> mSkyboxCommandList;

		RPtr<Mesh> mSkyboxMesh;
		SPtr<rt::Mesh> mSkyboxMesh_rt;

		UPtr<Skybox> mSkybox;
		SPtr<rt::Skybox> mSkybox_rt;
		bool mIsSkyboxUpdated;
	};
} // namespace cube
