#include "RendererManager.h"

#include "Platform.h"

#include "../EngineCore.h"
#include "../LogWriter.h"
#include "RenderingThread.h"
#include "Mesh.h"
#include "Renderer3D.h"
#include "CameraRenderer3D.h"
#include "Light/DirectionalLight.h"
#include "Light/PointLight.h"
#include "Material/Shader.h"
#include "Material/Material.h"
#include "Material/MaterialInstance.h"

namespace cube
{
	struct UBOGlobal
	{
		Vector3 cameraPos;
	};

	struct UBODirLight
	{
		int isExisted;
		Vector4 color;
		Vector3 direction;
	};

	struct UBOPointLights
	{
		int num;
		Vector4 color[RendererManager::maxPointLightNum];
		Vector3 position[RendererManager::maxPointLightNum];
	};

	void RendererManager::Initialize(RenderType type)
	{
		using namespace render;

		mWidth = platform::Platform::GetWindowWidth();
		mHeight = platform::Platform::GetWindowHeight();

		// TODO: multiple thread rendering
		mNumThreads = 1;

		switch(type) {
		case RenderType::Vulkan:
			mRenderDLib = platform::Platform::LoadDLib(CUBE_T("VulkanAPI"));
			break;

		default:
			ASSERTION_FAILED("Unknown renderer type ({0})", (int)type);
			return;
		}

		using CreateAPIFunction = render::RenderAPI*(*)();

		auto createAPIFunction = RCast(CreateAPIFunction)(mRenderDLib->GetFunction(CUBE_T("GetAPI")));
		// TODO: 좀 더 좋은 방법은 없을까?
		SPtr<render::RenderAPI> temp(createAPIFunction());
		mRenderAPI = std::move(temp);

		RenderAPIAttribute renderAPIAttr;
		renderAPIAttr.enableDebugLayer = true;
		mRenderAPI->Init(renderAPIAttr);

		// Get device
		DeviceAttribute deviceAttr;
		deviceAttr.GPUIndex = 0;
		deviceAttr.enableDebugLayer = false;
#ifdef _DEBUG
		deviceAttr.enableDebugLayer = true;
#endif // _DEBUG
		deviceAttr.debugName = "Main Device";
		mDevice = mRenderAPI->GetDevice(deviceAttr);

		CreateDepthBuffer();

		SwapChainAttribute swapChainAttr;
		swapChainAttr.width = mWidth;
		swapChainAttr.height = mHeight;
		swapChainAttr.colorBufferFormat = TextureFormat::RGBA_8_sRGB;
		swapChainAttr.depthBufferFormat = TextureFormat::D16_UNorm;
		swapChainAttr.vsync = false;
		swapChainAttr.bufferCount = 2;
		swapChainAttr.debugName = "SwapChain";
		mSwapChain = mDevice->CreateSwapChain(swapChainAttr);

		CreateRenderpass();

		// Get command lists
		CommandListAttribute commandListAttr;
		commandListAttr.usage = CommandListUsage::Graphics;
		commandListAttr.threadIndex = 0;
		commandListAttr.isSub = false;
		mMainCommandList = mDevice->GetCommandList(commandListAttr);
			
		commandListAttr.isSub = true;
		for(Uint32 i = 0; i < mNumThreads; i++) {
			commandListAttr.threadIndex = i;

			mCommandLists.push_back(mDevice->GetCommandList(commandListAttr));
			mCommandListsCurrentMaterialIndex.push_back(-1);
		}

		// Create camera renderer
		// TODO: multiple camera
		mCameraRenderer_NotRT = CameraRenderer3D::Create();
		mCameraRenderer = mCameraRenderer_NotRT->GetRenderObject();

		// Create Global / mPerObjectDescriptorSetLayout
		ShaderParametersLayoutAttribute paramsLayoutAttr;
		ShaderParameterInfo paramInfo;

		// Global shader parameters (layout)
		paramInfo.type = ShaderParameterType::ConstantBuffer;
		paramInfo.size = sizeof(UBOGlobal);
		paramInfo.count = 1;
		paramInfo.bindIndex = 0;
		paramInfo.isChangedPerFrame = false;
		paramInfo.debugName = "UBOGlobal parameter";
		paramsLayoutAttr.paramInfos.push_back(paramInfo);

		paramInfo.size = sizeof(UBODirLight);
		paramInfo.bindIndex = 1;
		paramInfo.debugName = "UBODirLight parameter";
		paramsLayoutAttr.paramInfos.push_back(paramInfo);

		paramInfo.size = sizeof(UBOPointLights);
		paramInfo.bindIndex = 2;
		paramInfo.debugName = "UBOPointLights parameter";
		paramsLayoutAttr.paramInfos.push_back(paramInfo);

		paramsLayoutAttr.debugName = "Global shader parameters layout";
		mGlobalShaderParametersLayout = mDevice->CreateShaderParametersLayout(paramsLayoutAttr);
		mGlobalShaderParameters = mGlobalShaderParametersLayout->CreateParameters();

		// Per-object shader parameters layout
		paramsLayoutAttr.paramInfos.clear();
			
		paramInfo.size = sizeof(UBOPerObject);
		paramInfo.bindIndex = 0;
		paramInfo.isChangedPerFrame = true;
		paramInfo.debugName = "UBOPerObjecrt parameter";
		paramsLayoutAttr.paramInfos.push_back(paramInfo);

		paramsLayoutAttr.debugName = "Per-object shader parameters layout";
		mPerObjectShaderParametersLayout = mDevice->CreateShaderParametersLayout(paramsLayoutAttr);

		mIsPrepared = true;
	}

	void RendererManager::ShutDown()
	{
		mRenderObjectTable.ReleaseAll();
		mRenderObjects.clear();

		mMaterialPipelines.clear();
		mMaterials.clear();
		mRenderers.clear();

		mGlobalShaderParametersLayout = nullptr;
		mGlobalShaderParameters = nullptr;

		mPerObjectShaderParametersLayout = nullptr;

		mCameraRenderer = nullptr;
		mCameraRenderer_NotRT = nullptr;

		mCommandListsCurrentMaterialIndex.clear();
		mCommandLists.clear();
		mMainCommandList = nullptr;
			
		mRenderPass = nullptr;
		mDepthStencilRenderTarget = nullptr;
		mColorRenderTarget = nullptr;

		mSwapChain = nullptr;

		mDepthBufferTextureView = nullptr;
		mDepthBufferTexture = nullptr;

		mDevice = nullptr;

		mRenderAPI = nullptr;
		mRenderDLib = nullptr;
	}

	HMaterial RendererManager::RegisterMaterial(UPtr<Material>&& material)
	{
		HMaterial mat = _registerRenderObject(std::move(material));

		RenderingThread::QueueTask([this, mat_rt = mat->GetRenderObject()]() {
			Lock(mMaterialsMutex);

			mat_rt->mIndex = (int)mMaterials.size();
			mMaterials.push_back(mat_rt);
			mMaterialPipelines.push_back(CreatePipeline(mat_rt));
		});

		return mat;
	}

	UPtr<Material> RendererManager::UnregisterMaterial(HMaterial& material)
	{
		SPtr<rt::Material> mat_rt = material->GetRenderObject();
		int index = mat_rt->mIndex;
		CHECK(index != -1, "This material is not registed.");

		RenderingThread::QueueTask([this, mat_rt]() {
			Lock(mMaterialsMutex);

			int index = mat_rt->mIndex;

			int lastIndex = mMaterials.back()->mIndex;
			mMaterials[lastIndex]->mIndex = index;

			mMaterials[index] = nullptr;

			std::swap(mMaterials[index], mMaterials[lastIndex]);
			std::swap(mMaterialPipelines[index], mMaterialPipelines[lastIndex]);
			mMaterials.pop_back();
			mMaterialPipelines.pop_back();

			mat_rt->mIndex = -1;
		});

		return _unregisterRenderObject(material);
	}

	HRenderer3D RendererManager::RegisterRenderer3D(UPtr<Renderer3D>&& renderer)
	{
		HRenderer3D r3d = _registerRenderObject(std::move(renderer));

		SPtr<rt::Renderer3D> r3d_rt = r3d->GetRenderObject();

		if(r3d_rt->mIndex != -1)
			return HRenderer3D();

		RenderingThread::QueueTask([this, r3d_rt]() {
			Lock(mRenderersMutex);

			mRenderers.push_back(r3d_rt);
			r3d_rt->mIndex = (int)mRenderers.size() - 1;
		});

		return r3d;
	}

	UPtr<Renderer3D> RendererManager::UnregisterRenderer3D(HRenderer3D& renderer)
	{
		SPtr<rt::Renderer3D> renderer_rt = renderer->GetRenderObject();

		int index = renderer_rt->mIndex;
		CHECK(index != -1, "This renderer is not registed.");

		RenderingThread::QueueTask([this, renderer_rt]() {
			Lock(mRenderersMutex);

			int index = renderer_rt->mIndex;

			int lastIndex = mRenderers.back()->mIndex;
			mRenderers[lastIndex]->mIndex = index;

			std::swap(mRenderers[index], mRenderers[lastIndex]);
			mRenderers.pop_back();

			renderer_rt->mIndex = -1;
		});

		return _unregisterRenderObject(renderer);
	}

	HDirectionalLight RendererManager::RegisterLight(UPtr<DirectionalLight>&& dirLight)
	{
		CHECK(mDirLight == nullptr, "DirectionalLight is already registed.");

		HDirectionalLight hDir = _registerRenderObject(std::move(dirLight));

		RenderingThread::QueueTask([this, dirLight_rt = hDir->GetRenderObject()]() {
			mDirLight = dirLight_rt;
		});

		return hDir;
	}

	UPtr<DirectionalLight> RendererManager::UnregisterLight(HDirectionalLight& dirLight)
	{
		CHECK(mDirLight == dirLight->GetRenderObject(), "This directional light is not registed.");

		RenderingThread::QueueTask([this]() {
			mDirLight = nullptr;
		});

		return _unregisterRenderObject(dirLight);
	}

	HPointLight RendererManager::RegisterLight(UPtr<PointLight>&& pointLight)
	{
		{
			Lock lock(mPointLightsMutex);

			CHECK(mPointLights.size() < maxPointLightNum, "PointLight cannot be registed more than {0}.", maxPointLightNum);
		}

		HPointLight hPoint = _registerRenderObject(std::move(pointLight));

		RenderingThread::QueueTask([this, pointLight_rt = hPoint->GetRenderObject()]() {
			Lock lock(mPointLightsMutex);

			mPointLights.push_back(pointLight_rt);
		});

		return hPoint;
	}

	UPtr<PointLight> RendererManager::UnregisterLight(HPointLight& pointLight)
	{
		RenderingThread::QueueTask([this, pointLight_rt = pointLight->GetRenderObject()]() {
			Lock lock(mPointLightsMutex);

			auto findIter = std::find(mPointLights.cbegin(), mPointLights.cend(), pointLight_rt);
			CHECK(findIter != mPointLights.cend(), "This point light is not registed.");

			mPointLights.erase(findIter);
		});

		return _unregisterRenderObject(pointLight);
	}

	SPtr<CameraRenderer3D> RendererManager::GetCameraRenderer3D()
	{
		return mCameraRenderer_NotRT;
	}

	void RendererManager::StartFrame()
	{
		mDevice->StartFrame();
	}

	void RendererManager::DrawAll()
	{
		using namespace render;

		if(mIsPrepared == false)
			return;

		mSwapChain->AcquireNextImage();

		RewriteCommandBuffer();

		SPtr<Fence> fence = mDevice->SubmitCommandListWithFence(mMainCommandList);
			
		FenceWaitResult res = fence->Wait(15.0f);
		CHECK(res == FenceWaitResult::Success, "Failed to submit main command list ({0}).", (int)res);

		mSwapChain->Present();
	}

	void RendererManager::Resize(Uint32 width, Uint32 height)
	{
		if(mIsPrepared == false)
			return;

		mWidth = width;
		mHeight = height;

		CreateDepthBuffer();

		mSwapChain->Resize(mWidth, mHeight, mVsync);

		CreateRenderpass();

		mIsPrepared = true;
	}

	void RendererManager::SetVsync(bool vsync)
	{
		mVsync = vsync;

		mSwapChain->Resize(mWidth, mHeight, mVsync);
	}

	void RendererManager::CreateDepthBuffer()
	{
		using namespace render;

		TextureAttribute attr;
		attr.usage = ResourceUsage::Default;
		attr.type = TextureType::Texture2D;
		attr.format = TextureFormat::D16_UNorm;
		attr.bindTypeFlags = TextureBindTypeFlag::DepthStencil;
		attr.width = mWidth;
		attr.height = mHeight;
		attr.depth = 1;
		attr.arraySize = 1;
		attr.mipLevels = 1;
		attr.isDedicated = true;
		attr.debugName = "Depth buffer texture";

		mDepthBufferTexture = mDevice->CreateTexture(attr);
		// Disable stencil bit (Set only depth bit)
		auto viewAttr = mDepthBufferTexture->GetDefaultViewAttr();
		viewAttr.componentFlags = TextureViewComponentFlag::Depth;
		mDepthBufferTextureView = mDepthBufferTexture->CreateView(viewAttr);
	}

	void RendererManager::CreateRenderpass()
	{
		using namespace render;

		// Color render target
		RenderTargetAttribute rtAttr;
		rtAttr.textureView = nullptr;
		rtAttr.isSwapChain = true;
		rtAttr.swapChain = mSwapChain;
		rtAttr.loadOp = LoadOperator::Clear;
		rtAttr.storeOp = StoreOperator::Store;
		rtAttr.stencilLoadOp = LoadOperator::DontCare;
		rtAttr.stencilStoreOp = StoreOperator::DontCare;
		rtAttr.defaultColor[0] = 0.02f;
		rtAttr.defaultColor[1] = 0.02f;
		rtAttr.defaultColor[2] = 0.02f;
		rtAttr.defaultColor[3] = 0.0f;
		rtAttr.defaultDepth = 1.0f;
		rtAttr.defaultStencil = 0;
		rtAttr.debugName = "Color render target";
		mColorRenderTarget = mDevice->CreateRenderTarget(rtAttr);

		// Depth-stencil render target
		rtAttr.textureView = mDepthBufferTextureView;
		rtAttr.isSwapChain = false;
		rtAttr.swapChain = nullptr;
		rtAttr.loadOp = LoadOperator::Clear;
		rtAttr.storeOp = StoreOperator::DontCare;
		rtAttr.stencilLoadOp = LoadOperator::DontCare;
		rtAttr.stencilStoreOp = StoreOperator::DontCare;
		rtAttr.defaultColor[0] = 0.0f;
		rtAttr.defaultColor[1] = 0.0f;
		rtAttr.defaultColor[2] = 0.0f;
		rtAttr.defaultColor[3] = 0.0f;
		rtAttr.defaultDepth = 1.0f;
		rtAttr.defaultStencil = 0;
		rtAttr.debugName = "Depth-stencil render target";
		mDepthStencilRenderTarget = mDevice->CreateRenderTarget(rtAttr);

		// Render pass / sub pass
		Subpass subpass;
		subpass.colorOutputs.push_back(0); // Color render target
		subpass.depthStencilOutput = 1; // Depth-stencil render target
		subpass.debugName = "Subpass";

		RenderPassAttribute rpAttr;
		rpAttr.renderTargets.push_back(mColorRenderTarget);        // 0
		rpAttr.renderTargets.push_back(mDepthStencilRenderTarget); // 1
		rpAttr.subpasses.push_back(subpass);
		rpAttr.width = mWidth;
		rpAttr.height = mHeight;
		rpAttr.debugName = "Render pass";
		mRenderPass = mDevice->CreateRenderPass(rpAttr);
	}

	void RendererManager::RewriteCommandBuffer()
	{
		using namespace render;

		Viewport vp;
		vp.width = SCast(float)(mWidth);
		vp.height = SCast(float)(mHeight);
		vp.minDepth = 0.0f;
		vp.maxDepth = 1.0f;
		vp.x = 0;
		vp.y = 0;

		Rect2D scissor;
		scissor.x = 0;
		scissor.y = 0;
		scissor.width = mWidth;
		scissor.height = mHeight;

		Rect2D renderArea;
		renderArea.x = 0;
		renderArea.y = 0;
		renderArea.width = mWidth;
		renderArea.height = mHeight;

		// Update global
		UBOGlobal uboGlobal;
		uboGlobal.cameraPos = mCameraRenderer->GetPosition();
		mGlobalShaderParameters->UpdateParameter(0, &uboGlobal, sizeof(UBOGlobal));

		// Update directional lights
		UBODirLight uboDirLight;
		if(mDirLight != nullptr) {
			uboDirLight.color = mDirLight->GetColor();
			uboDirLight.direction = mDirLight->GetDirection();
			uboDirLight.isExisted = 1;
		} else {
			uboDirLight.isExisted = 0;
		}
		mGlobalShaderParameters->UpdateParameter(1, &uboDirLight, sizeof(UBODirLight));

		// Update point lights
		UBOPointLights uboPointLights;
		uboPointLights.num = (int)mPointLights.size();
		for(int i = 0; i < uboPointLights.num; i++) {
			uboPointLights.color[i] = mPointLights[i]->GetColor();
			uboPointLights.position[i] = mPointLights[i]->GetPosition();
		}
		mGlobalShaderParameters->UpdateParameter(2, &uboPointLights, sizeof(UBOPointLights));

		// Prepare command lists
		for(Uint64 i = 0; i < mCommandLists.size(); i++) {
			mCommandLists[i]->Reset();
			mCommandLists[i]->Begin();

			mCommandLists[i]->SetRenderPass(mRenderPass, 0);

			mCommandLists[i]->SetViewports(1, &vp);
			mCommandLists[i]->SetScissors(1, &scissor);

			mCommandListsCurrentMaterialIndex[i] = -1;
		}

		// TODO: multiple thread rendering
		for(auto& renderer : mRenderers) {
			DrawRenderer3D(0, renderer);
		}

		for(auto& cmd : mCommandLists) {
			cmd->End();
		}

		// Main command buffer
		mMainCommandList->Reset();

		mMainCommandList->Begin();

		mMainCommandList->SetRenderPass(mRenderPass, 0);

		mMainCommandList->ExecuteCommands((Uint32)mCommandLists.size(), mCommandLists.data());

		mMainCommandList->End();
	}

	void RendererManager::DrawRenderer3D(Uint32 commandListIndex, SPtr<rt::Renderer3D>& renderer)
	{
		using namespace render;

		SPtr<CommandList>& cmd = mCommandLists[commandListIndex];
		int& currentMatIndex = mCommandListsCurrentMaterialIndex[commandListIndex];

		renderer->PrepareDraw(cmd, mCameraRenderer);

		const Vector<SubMesh>& subMeshes = renderer->mMesh->GetSubMeshes();
		for(Uint32 i = 0; i < subMeshes.size(); i++) {
			SPtr<rt::MaterialInstance> matIns = renderer->mMaterialInses[i];
			int matIndex = matIns->GetMaterial()->mIndex;

			if(matIndex != currentMatIndex) {
				cmd->SetGraphicsPipelineState(mMaterialPipelines[matIndex]);
				currentMatIndex = matIndex;
			}

			SPtr<ShaderParameters> matInsShaderPameters = matIns->GetShaderParameters();

			cmd->BindShaderParameters(0, mGlobalShaderParameters);
			cmd->BindShaderParameters(1, matInsShaderPameters);
			cmd->BindShaderParameters(2, renderer->mShaderParameters);

			cmd->DrawIndexed(
				SCast(Uint32)(subMeshes[i].indexCount),
				SCast(Uint32)(subMeshes[i].indexOffset),
				SCast(Uint32)(subMeshes[i].vertexOffset),
				1, 0);
		}
	}

	SPtr<render::GraphicsPipelineState> RendererManager::CreatePipeline(SPtr<rt::Material> material)
	{
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
		attr.rasterizerState.cullMode = CullMode::Back;
		attr.rasterizerState.frontFace = TriangleFrontFace::Clockwise;

		attr.depthStencilState.enableDepthTest = true;
		attr.depthStencilState.enableDepthWrite = true;
		attr.depthStencilState.depthCompareFunc = ComparisonFunction::Less;

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
		//       왜냐하면 Vulkan에서는 color만 blend를 적용하기 땜누
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
		attr.renderTargetFormats[0] = TextureFormat::RGBA_8_sRGB;

		attr.depthStencilFormat = TextureFormat::D16_UNorm;

		auto& shaders = material->GetShaders();
		for(auto& shader : shaders) {
			switch(shader->GetType()) {
				case ShaderType::Vertex:   attr.vertexShader = shader->GetRenderShader(); break;
				case ShaderType::Pixel:    attr.pixelShader = shader->GetRenderShader(); break;
				case ShaderType::Geometry: attr.geometryShader = shader->GetRenderShader(); break;
				case ShaderType::Hull:     attr.hullShader = shader->GetRenderShader(); break;
				case ShaderType::Domain:   attr.domainShader = shader->GetRenderShader(); break;
				// case ShaderType::Compute:  attr.computeSh = shader->GetRenderShader(); break;

				default: ASSERTION_FAILED("Unknown Shader type ({0}).", (int)(shader->GetType()));
					break;
			}
		}

		attr.renderPass = mRenderPass;

		attr.shaderParameterLayouts.push_back(mGlobalShaderParametersLayout);
		attr.shaderParameterLayouts.push_back(material->GetShaderParametersLayout());
		attr.shaderParameterLayouts.push_back(mPerObjectShaderParametersLayout);

		attr.debugName = "Pipeline";

		return mDevice->CreateGraphicsPipelineState(attr);
	}
} // namespace cube
