#include "SkyboxSystem.h"

#include "../RendererManager.h"
#include "../RenderingThread.h"
#include "../Material/Shader.h"
#include "../Mesh.h"
#include "../BaseMeshGenerator.h"

namespace cube
{
	void SkyboxSystem::Initialize()
	{
		SPtr<render::Device> device = mManager.GetDevice();
		
		mSkyboxVertexShader = Shader::Load(CUBE_T("../../../SampleResources/Shaders/Skybox.vs"));
		mSkyboxPixelShader = Shader::Load(CUBE_T("../../../SampleResources/Shaders/Skybox.fs"));

		// Create SkyboxShaderParametersLayout
		render::ShaderParametersLayoutAttribute paramsLayoutAttr;
		render::ShaderParameterInfo paramInfo;
		
		paramInfo.type = render::ShaderParameterType::SampledImage;
		paramInfo.size = 0;
		paramInfo.count = 1;
		paramInfo.bindIndex = 0;
		paramInfo.isChangedPerFrame = false;
		paramInfo.debugName = "Skybox parameter";
		paramsLayoutAttr.paramInfos.push_back(paramInfo);

		paramsLayoutAttr.debugName = "Skybox shader parameters layout";
		mSkyboxShaderParametersLayout = device->CreateShaderParametersLayout(paramsLayoutAttr);
		mSkyboxShaderParameters = mSkyboxShaderParametersLayout->CreateParameters();

		CreateSkyboxPipeline();

		mSkyboxMesh = BaseMeshGenerator::GetBoxMesh();
		mSkyboxMesh_rt = mSkyboxMesh->GetRenderObject();

		// Command List
		render::CommandListAttribute commandListAttr;
		commandListAttr.usage = render::CommandListUsage::Graphics;
		commandListAttr.threadIndex = 0;
		commandListAttr.isSub = true;
		mSkyboxCommandList = device->GetCommandList(commandListAttr);
	}

	void SkyboxSystem::ShutDown()
	{
		mSkyboxCommandList = nullptr;

		mSkybox_rt = nullptr;
		mSkybox = nullptr;

		mSkyboxMesh_rt = nullptr;
		mSkyboxMesh = nullptr;

		mSkyboxPipeline = nullptr;
		mSkyboxShaderParameters = nullptr;
		mSkyboxShaderParametersLayout = nullptr;

		mSkyboxVertexShader = nullptr;
		mSkyboxPixelShader = nullptr;
	}

	HSkybox SkyboxSystem::RegisterSkybox(UPtr<Skybox>&& skybox)
	{
		CHECK(mSkybox == nullptr, "Skybox is already registered.");

		mSkybox = std::move(skybox);

		HSkybox hSkybox = mManager._getRenderObjectTable().CreateNewHandler(mSkybox.get());

		RenderingThread::QueueTask([this, skybox_rt = mSkybox->GetRenderObject()]() {
			mSkybox_rt = skybox_rt;

			mIsSkyboxUpdated = true;
		});

		return hSkybox;
	}

	UPtr<Skybox> SkyboxSystem::UnregisterSkybox(HSkybox skybox)
	{
		CHECK(mSkybox_rt == skybox->GetRenderObject(), "This skybox is not registered.");

		RenderingThread::QueueTask([this]() {
			mSkybox = nullptr;
		});

		mManager._getRenderObjectTable().ReleaseHandler(skybox);

		return std::move(mSkybox);
	}

	void SkyboxSystem::DrawSkybox(render::Viewport& vp, render::Rect2D& scissor)
	{
		if(mSkybox_rt == nullptr || mIsSkyboxUpdated == false)
			return;

		mSkyboxShaderParameters->UpdateParameter(0, mSkybox_rt->GetTextureView(), mSkybox_rt->GetSampler());

		RewriteCommandList(vp, scissor);
	}

	void SkyboxSystem::CreateSkyboxPipeline()
	{
		SPtr<render::Device> device = mManager.GetDevice();

		using namespace render;

		GraphicsPipelineStateAttribute attr;
		attr.inputLayouts.resize(1);
		attr.inputLayouts[0].size = sizeof(Vertex);
		attr.inputLayouts[0].elements.resize(4);

		// Position
		attr.inputLayouts[0].elements[0].format = TextureFormat::RGBA_32_Float;
		attr.inputLayouts[0].elements[0].offset = 0;
		// Color
		attr.inputLayouts[0].elements[1].format = TextureFormat::RGBA_32_Float;
		attr.inputLayouts[0].elements[1].offset = 16;
		// Normal
		attr.inputLayouts[0].elements[2].format = TextureFormat::RGBA_32_Float;
		attr.inputLayouts[0].elements[2].offset = 32;
		// Texture UV
		attr.inputLayouts[0].elements[3].format = TextureFormat::RGBA_32_Float;
		attr.inputLayouts[0].elements[3].offset = 48;

		attr.primitiveTopology = PrimitiveTopology::TriangleList;

		attr.rasterizerState.fillMode = FillMode::Fill;
		attr.rasterizerState.cullMode = CullMode::Front;
		attr.rasterizerState.frontFace = TriangleFrontFace::Clockwise;

		attr.depthStencilState.enableDepthTest = false;
		attr.depthStencilState.enableDepthWrite = false;
		attr.depthStencilState.depthCompareFunc = ComparisonFunction::LessEqual;
		attr.depthStencilState.enableStencilTest = false;
		StencilState stencilState;
		stencilState.depthFailOp = StencilOperator::Keep;
		stencilState.failOp = StencilOperator::Keep;
		stencilState.passOp = StencilOperator::Keep;
		stencilState.compareFunc = ComparisonFunction::Always;
		stencilState.readMask = 0;
		stencilState.writeMask = 0;
		attr.depthStencilState.frontFace = stencilState;
		attr.depthStencilState.backFace = stencilState;

		// TODO: 일단 colorRenderTArget만
		//       왜냐하면 Vulkan에서는 color만 blend를 적용하기 때문
		//       근데 그냥 depth에도 하면 안 되나?
		attr.blendState.renderTargets.resize(1);
		attr.blendState.renderTargets[0].enableBlend = false;
		attr.blendState.renderTargets[0].srcBlend = BlendFactor::Zero;
		attr.blendState.renderTargets[0].dstBlend = BlendFactor::Zero;
		attr.blendState.renderTargets[0].blendOp = BlendOperator::Add;
		attr.blendState.renderTargets[0].srcAlphaBlend = BlendFactor::Zero;
		attr.blendState.renderTargets[0].dstAlphaBlend = BlendFactor::Zero;
		attr.blendState.renderTargets[0].alphaBlendOp = BlendOperator::Add;
		attr.blendState.renderTargets[0].writeMask = ColorWriteMaskFlag::All;

		attr.renderTargetFormats.resize(1);
		attr.renderTargetFormats[0] = TextureFormat::RGBA_8_UNorm;

		attr.depthStencilFormat = TextureFormat::D16_UNorm;

		attr.vertexShader = mSkyboxVertexShader->GetRenderShader();
		attr.pixelShader = mSkyboxPixelShader->GetRenderShader();

		attr.renderPass = mManager.GetRenderPass();

		attr.shaderParameterLayouts.push_back(mManager.GetGlobalShaderParametersLayout());
		attr.shaderParameterLayouts.push_back(mSkyboxShaderParametersLayout);

		attr.debugName = "Skybox Pipeline";

		mSkyboxPipeline = device->CreateGraphicsPipelineState(attr);
	}

	void SkyboxSystem::RewriteCommandList(render::Viewport& vp, render::Rect2D& scissor)
	{
		SPtr<render::Device> device = mManager.GetDevice();

		mSkyboxCommandList->Reset();
		mSkyboxCommandList->Begin();

		mSkyboxCommandList->SetRenderPass(mManager.GetRenderPass(), 0);

		mSkyboxCommandList->SetViewports(1, &vp);
		mSkyboxCommandList->SetScissors(1, &scissor);

		mSkyboxCommandList->SetGraphicsPipelineState(mSkyboxPipeline);
		mSkyboxCommandList->BindShaderParameters(0, mManager.GetGlobalShaderParameters());
		mSkyboxCommandList->BindShaderParameters(1, mSkyboxShaderParameters);

		SPtr<render::Buffer> meshBuf = mSkyboxMesh_rt->GetMeshBuffer();
		const Vector<SubMesh>& subMeshes = mSkyboxMesh_rt->GetSubMeshes();

		Uint32 vertexOffset = SCast(Uint32)(mSkyboxMesh_rt->GetVertexOffset());

		mSkyboxCommandList->BindVertexBuffers(0, 1, &meshBuf, &vertexOffset);
		mSkyboxCommandList->BindIndexBuffer(meshBuf, SCast(Uint32)(mSkyboxMesh_rt->GetIndexOffset()));
		mSkyboxCommandList->DrawIndexed(SCast(Uint32)(subMeshes[0].indexCount),
							 SCast(Uint32)(subMeshes[0].indexOffset),
							 SCast(Uint32)(subMeshes[0].vertexOffset),
							 1, 0);

		mSkyboxCommandList->End();
	}
} // namespace cube
