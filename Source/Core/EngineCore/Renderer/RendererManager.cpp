#include "RendererManager.h"

#include "Platform.h"

#include "../EngineCore.h"
#include "../LogWriter.h"
#include "../GameThread.h"
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
	namespace core
	{
		struct UBOGlobal
		{
			Vector3 cameraPos;
		};

		struct UBODirLight
		{
			Vector4 color;
			Vector3 direction;
			int isExisted;
		};

		struct UBOPointLights
		{
			int num;
			Vector4 color[RendererManager::maxPointLightNum];
			Vector3 position[RendererManager::maxPointLightNum];
		};

		void RendererManager::Initialize(EngineCore* eCore)
		{
			mECore = eCore;
		}

		void RendererManager::ShutDown()
		{
			mRenderers.clear();

			CUBE_LOG(LogType::Info, "Destroyed RendererManager");
		}

		void RendererManager::Prepare(RenderType type)
		{
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

			auto createAPIFunction = RCast(CreateAPIFunction)(mRenderDLib->GetFunction(CUBE_T("CreateAPI")));
			// TODO: 좀 더 좋은 방법은 없을까?
			SPtr<render::RenderAPI> temp(createAPIFunction());
			mRenderAPI = std::move(temp);

			mRenderAPI->Init();

			// Graphics queue
			mGraphicsQueue = mRenderAPI->GetQueue(render::QueueTypeBits::GraphicsBit, 0);

			CreateDepthBuffer();

			mSwapchain = mRenderAPI->CreateSwapchain();
			mSwapchain->Recreate(2, mWidth, mHeight, true);

			CreateRenderpass();

			// Get a main command buffer
			mMainCommandBuffer = mRenderAPI->CreateCommandBuffer();
			mMainCommandBufferSubmitFence = mRenderAPI->CreateFence();

			for(uint32_t i = 0; i < mNumThreads; i++) {
				mCommandBuffers.push_back(mRenderAPI->CreateCommandBuffer(false));
				mCommandBuffersCurrentMaterialIndex.push_back(-1);
			}

			mGetImageSemaphore = mRenderAPI->CreateSemaphore();

			// Create camera renderer
			// TODO: multiple camera
			mCameraRenderer_NotRT = CameraRenderer3D::Create();
			mCameraRenderer = mCameraRenderer_NotRT->GetRenderObject_RT();

			// Create Global / mPerObjectDescriptorSetLayout
			render::DescriptorSetInitializer descSetInit;

			descSetInit.descriptors.push_back({render::ShaderTypeBits::Fragment, render::DescriptorType::UniformBuffer, 0, 1}); // global
			render::BufferInitializer globalUBOBufInit;
			globalUBOBufInit.type = render::BufferTypeBits::Uniform;
			globalUBOBufInit.bufferDatas.push_back({nullptr, sizeof(UBOGlobal)});
			mGlobalUBOBuffer = mRenderAPI->CreateBuffer(globalUBOBufInit);
			mGlobalUBOBuffer->Map();

			descSetInit.descriptors.push_back({render::ShaderTypeBits::Fragment, render::DescriptorType::UniformBuffer, 1, 1}); // dirLight
			render::BufferInitializer dirLightBufInit;
			dirLightBufInit.type = render::BufferTypeBits::Uniform;
			dirLightBufInit.bufferDatas.push_back({nullptr, sizeof(UBODirLight)});
			mDirLightBuffer = mRenderAPI->CreateBuffer(dirLightBufInit);
			mDirLightBuffer->Map();

			descSetInit.descriptors.push_back({render::ShaderTypeBits::Fragment, render::DescriptorType::UniformBuffer, 2, 1}); // pointLights
			render::BufferInitializer pointLightBufInit;
			pointLightBufInit.type = render::BufferTypeBits::Uniform;
			pointLightBufInit.bufferDatas.push_back({nullptr, sizeof(UBOPointLights)});
			mPointLightsBuffer = mRenderAPI->CreateBuffer(pointLightBufInit);
			mPointLightsBuffer->Map();

			mGlobalDescriptorSetLayout = mRenderAPI->CreateDescriptorSetLayout(descSetInit);
			mGlobalDescriptorSet = mRenderAPI->CreateDescriptorSet(mGlobalDescriptorSetLayout);

			descSetInit.descriptors.clear();
			descSetInit.descriptors.push_back({render::ShaderTypeBits::Vertex, render::DescriptorType::UniformBuffer, 0, 1});
			mPerObjectDescriptorSetLayout = mRenderAPI->CreateDescriptorSetLayout(descSetInit);

			mIsPrepared = true;
		}

		HMaterial RendererManager::RegisterMaterial(SPtr<Material>& material)
		{
			SPtr<MaterialData> matDataPtr = std::make_shared<MaterialData>();
			matDataPtr->data = std::move(material);
			matDataPtr->data->mMyHandler = HMaterial(matDataPtr);

			SPtr<Material_RT> mat_rt = matDataPtr->data->GetRenderObject_RT();
			GameThread::QueueTask([this, mat_rt]() {
				Lock(mMaterialsMutex);

				mat_rt->mIndex = (int)mMaterials.size();
				mMaterials.push_back(mat_rt);
				mMaterialPipelines.push_back(CreatePipeline(mat_rt));
			});

			HMaterial hMat = HMaterial(matDataPtr);
			return hMat;
		}

		void RendererManager::UnregisterMaterial(HMaterial& material)
		{
			SPtr<Material_RT> mat_rt = material->GetRenderObject_RT();
			int index = mat_rt->mIndex;
			CHECK(index != -1, "This material is not registed.");

			GameThread::QueueTask([this, mat_rt]() {
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
		}

		void RendererManager::RegisterRenderer3D(SPtr<Renderer3D>& renderer)
		{
			SPtr<Renderer3D_RT> renderer_rt = renderer->GetRenderObject_RT();

			if(renderer_rt->mIndex != -1)
				return;

			GameThread::QueueTask([this, renderer_rt]() {
				Lock(mRenderersMutex);

				mRenderers.push_back(renderer_rt);
				renderer_rt->mIndex = (int)mRenderers.size() - 1;
			});
		}

		void RendererManager::UnregisterRenderer3D(SPtr<Renderer3D>& renderer)
		{
			SPtr<Renderer3D_RT> renderer_rt = renderer->GetRenderObject_RT();

			int index = renderer_rt->mIndex;
			CHECK(index != -1, "This renderer is not registed.");

			GameThread::QueueTask([this, renderer_rt]() {
				Lock(mRenderersMutex);

				int index = renderer_rt->mIndex;

				int lastIndex = mRenderers.back()->mIndex;
				mRenderers[lastIndex]->mIndex = index;

				std::swap(mRenderers[index], mRenderers[lastIndex]);
				mRenderers.pop_back();

				renderer_rt->mIndex = -1;
			});
		}

		void RendererManager::RegisterLight(SPtr<DirectionalLight>& dirLight)
		{
			CHECK(mDirLight == nullptr, "DirectionalLight is already registed.");

			SPtr<DirectionalLight_RT> dirLight_rt = dirLight->GetRenderObject_RT();
			GameThread::QueueTask([this, dirLight_rt]() {
				mDirLight = dirLight_rt;
			});
		}

		void RendererManager::UnregisterLight(SPtr<DirectionalLight>& dirLight)
		{
			CHECK(mDirLight == dirLight->GetRenderObject_RT(), "This directional light is not registed.");

			GameThread::QueueTask([this]() {
				mDirLight = nullptr;
			});
		}

		void RendererManager::RegisterLight(SPtr<PointLight>& pointLight)
		{
			{
				Lock lock(mPointLightsMutex);

				CHECK(mPointLights.size() < maxPointLightNum, "PointLight cannot be registed more than 50.");
			}

			SPtr<PointLight_RT> pointLight_rt = pointLight->GetRenderObject_RT();
			GameThread::QueueTask([this, pointLight_rt]() {
				Lock lock(mPointLightsMutex);

				mPointLights.push_back(pointLight_rt);
			});
		}

		void RendererManager::UnregisterLight(SPtr<PointLight>& pointLight)
		{
			SPtr<PointLight_RT> pointLight_rt = pointLight->GetRenderObject_RT();
			GameThread::QueueTask([this, pointLight_rt]() {
				Lock lock(mPointLightsMutex);

				auto findIter = std::find(mPointLights.cbegin(), mPointLights.cend(), pointLight_rt);
				CHECK(findIter != mPointLights.cend(), "This point light is not registed.");

				mPointLights.erase(findIter);
			});
		}

		SPtr<Renderer3D> RendererManager::CreateRenderer3D()
		{
			return Renderer3D::Create();
		}

		SPtr<CameraRenderer3D> RendererManager::GetCameraRenderer3D()
		{
			return mCameraRenderer_NotRT;
		}

		void RendererManager::DrawAll()
		{
			if(mIsPrepared == false)
				return;

			mSwapchain->AcquireNextImageIndex(mGetImageSemaphore);

			RewriteCommandBuffer();

			mMainCommandBufferSubmitFence->Reset();
			auto temp = std::make_pair(mGetImageSemaphore, render::PipelineStageBits::ColorAttachmentOutputBit);
			mMainCommandBuffer->Submit(mGraphicsQueue, 1, &temp, 0, nullptr, mMainCommandBufferSubmitFence);
			
			bool r = mMainCommandBufferSubmitFence->Wait(100000000);
			CHECK(r == true, "Main command buffer submit fence time out");

			mSwapchain->Present(0, nullptr);
		}

		void RendererManager::Resize(uint32_t width, uint32_t height)
		{
			if(mIsPrepared == false)
				return;

			mWidth = width;
			mHeight = height;

			CreateDepthBuffer();

			mSwapchain->Recreate(2, width, height, mVsync);

			CreateRenderpass();

			mIsPrepared = true;
		}

		void RendererManager::SetVsync(bool vsync)
		{
			mVsync = vsync;

			mSwapchain->Recreate(2, mWidth, mHeight, vsync);
		}

		void RendererManager::CreateDepthBuffer()
		{
			using namespace render;

			ImageInitializer imageInit;
			imageInit.type = ImageType::Image2D;
			imageInit.format = DataFormat::D16_Unorm;
			imageInit.width = mWidth;
			imageInit.height = mHeight;
			imageInit.depth = 1;
			imageInit.mipLevels = 1;
			imageInit.usage = ImageUsageBits::DepthStencilAttachmentBit;
			mDepthBufferImage = mRenderAPI->CreateImage(imageInit);
			mDepthBufferImageView = mDepthBufferImage->GetImageView(DataFormat::D16_Unorm, ImageAspectBits::Depth, ImageViewType::Image2D);
		}

		void RendererManager::CreateRenderpass()
		{
			using namespace render;

			Color c;
			DepthStencilValue v;

			render::RenderPassInitializer renderPassInit;
			// Depth buffer attachment
			render::RenderPassInitializer::Attachment att;
			att.imageView = mDepthBufferImageView;
			att.format = DataFormat::D16_Unorm;
			att.loadOp = LoadOperator::Clear;
			att.storeOp = StoreOperator::DontCare;
			c.float32 = {0, 0, 0, 0};
			att.clearColor = c;
			att.initialLayout = ImageLayout::Undefined;
			att.finalLayout = ImageLayout::DepthStencilAttachmentOptimal;

			att.isDepthStencil = true;
			att.stencilLoadOp = LoadOperator::DontCare;
			att.stencilStoreOp = StoreOperator::DontCare;
			v.depth = 1.0f;
			v.stencil = 0;
			att.clearDepthStencil = v;
			renderPassInit.attachments.push_back(att);

			// Swapchain attachment
			render::RenderPassInitializer::SwapchainAttachment swapAtt;
			swapAtt.swapchain = mSwapchain;
			swapAtt.loadOp = LoadOperator::Clear;
			swapAtt.storeOp = StoreOperator::Store;
			c.float32 = {0.02f, 0.02f, 0.02f, 0};
			swapAtt.clearColor = c;
			swapAtt.initialLayout = ImageLayout::Undefined;
			swapAtt.finalLayout = ImageLayout::PresentSource;
			renderPassInit.hasSwapchain = true;
			renderPassInit.swapchainAttachment = swapAtt;

			// Subpass
			render::Subpass subpass;
			subpass.mColors.push_back({1, ImageLayout::ColorAttachmentOptimal});
			subpass.mDepthStencil.index = 0;
			subpass.mDepthStencil.layout = ImageLayout::DepthStencilAttachmentOptimal;
			renderPassInit.subpasses.push_back(subpass);

			mRenderPass = mRenderAPI->CreateRenderPass(renderPassInit);
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

			mGlobalUBOBuffer->UpdateBufferData(0, &uboGlobal, sizeof(UBOGlobal));
			render::BufferInfo globalUBOBufInfo = mGlobalUBOBuffer->GetInfo(0);
			mGlobalDescriptorSet->WriteBufferInDescriptor(0, 1, &globalUBOBufInfo);

			// Update directional lights
			UBODirLight uboDirLight;
			if(mDirLight != nullptr) {
				uboDirLight.color = mDirLight->GetColor();
				uboDirLight.direction = mDirLight->GetDirection();
				uboDirLight.isExisted = 1;
			} else {
				uboDirLight.isExisted = 0;
			}
			mDirLightBuffer->UpdateBufferData(0, &uboDirLight, sizeof(UBODirLight));

			render::BufferInfo bufInfo = mDirLightBuffer->GetInfo(0);
			mGlobalDescriptorSet->WriteBufferInDescriptor(1, 1, &bufInfo);

			// Update point lights
			UBOPointLights uboPointLights;
			uboPointLights.num = (int)mPointLights.size();
			for(int i = 0; i < uboPointLights.num; i++) {
				uboPointLights.color[i] = mPointLights[i]->GetColor();
				uboPointLights.position[i] = mPointLights[i]->GetPosition();
			}
			mPointLightsBuffer->UpdateBufferData(0, &uboPointLights, sizeof(UBOPointLights));
			
			render::BufferInfo pointLightBufInfo = mPointLightsBuffer->GetInfo(0);
			mGlobalDescriptorSet->WriteBufferInDescriptor(2, 1, &pointLightBufInfo);

			// Prepare command buffers
			for(uint32_t i = 0; i < mCommandBuffers.size(); i++) {
				mCommandBuffers[i]->Reset();
				mCommandBuffers[i]->Begin();

				mCommandBuffers[i]->SetRenderPass(mRenderPass, renderArea);

				mCommandBuffers[i]->SetViewport(0, 1, &vp);
				mCommandBuffers[i]->SetScissor(0, 1, &scissor);

				mCommandBuffersCurrentMaterialIndex[i] = -1;
			}

			// TODO: multiple thread rendering
			for(auto& renderer : mRenderers) {
				DrawRenderer3D(0, renderer);
			}

			for(auto& cmd : mCommandBuffers) {
				cmd->End();
			}

			// Main command buffer
			mMainCommandBuffer->Reset();

			mMainCommandBuffer->Begin();

			mMainCommandBuffer->SetRenderPass(mRenderPass, renderArea);

			mMainCommandBuffer->ExecuteCommands((uint32_t)mCommandBuffers.size(), mCommandBuffers.data());

			mMainCommandBuffer->End();
		}

		void RendererManager::DrawRenderer3D(uint32_t commandBufferIndex, SPtr<Renderer3D_RT>& renderer)
		{
			using namespace render;

			SPtr<CommandBuffer>& cmd = mCommandBuffers[commandBufferIndex];
			int& currentMatIndex = mCommandBuffersCurrentMaterialIndex[commandBufferIndex];

			renderer->PrepareDraw(cmd, mCameraRenderer);

			Vector<SubMesh>& subMeshes = renderer->mMesh->GetSubMeshes();
			for(uint32_t i = 0; i < subMeshes.size(); i++) {
				SPtr<MaterialInstance_RT> matIns = renderer->mMaterialInses[i];
				int matIndex = matIns->GetMaterial()->mIndex;

				if(matIndex != currentMatIndex) {
					cmd->BindGraphicsPipeline(mMaterialPipelines[matIndex]);
					currentMatIndex = matIndex;
				}

				SPtr<DescriptorSet> matInsDesc = matIns->GetDescriptorSet();

				cmd->BindDescriptorSets(PipelineType::Graphics, 0, 1, &mGlobalDescriptorSet);
				cmd->BindDescriptorSets(PipelineType::Graphics, 1, 1, &matInsDesc);
				cmd->BindDescriptorSets(render::PipelineType::Graphics, 2, 1, &(renderer->mDescriptorSet));

				cmd->DrawIndexed(
					SCast(uint32_t)(subMeshes[i].indexCount),
					SCast(uint32_t)(subMeshes[i].indexOffset),
					SCast(uint32_t)(subMeshes[i].vertexOffset),
					1, 0);
			}
		}

		SPtr<render::GraphicsPipeline> RendererManager::CreatePipeline(SPtr<Material_RT> material)
		{
			using namespace render;

			render::GraphicsPipelineInitializer initializer;

			uint32_t currentVertexOffset = 0;
			render::GraphicsPipelineInitializer::VertexInputAttribute attr;
			attr.location = 0; // Position data
			attr.format = DataFormat::R32G32B32A32_SFloat;
			attr.offset = 0;
			initializer.vertexInputAttributes.push_back(attr);

			attr.location = 1; // Color
			attr.format = DataFormat::R32G32B32A32_SFloat;
			attr.offset = 16;
			initializer.vertexInputAttributes.push_back(attr);

			attr.location = 2; // Normal
			attr.format = DataFormat::R32G32B32_SFloat;
			attr.offset = 32;
			initializer.vertexInputAttributes.push_back(attr);

			attr.location = 3; // Texture coordination
			attr.format = DataFormat::R32G32_SFloat;
			attr.offset = 48;
			initializer.vertexInputAttributes.push_back(attr);

			initializer.vertexSize = sizeof(Vertex);
			initializer.vertexTopology = VertexTopology::Triangle;

			initializer.rasterizer = {PolygonMode::Fill, PolygonFrontFace::Clockwise, CullMode::Back};

			render::GraphicsPipelineInitializer::ColorBlendAttachment colorAttr;
			colorAttr.blendEnable = false;
			colorAttr.srcColorBlendFactor = BlendFactor::Zero;
			colorAttr.dstColorBlendFactor = BlendFactor::Zero;
			colorAttr.colorBlendOp = BlendOperator::Add;
			colorAttr.srcAlphaBlendFactor = BlendFactor::Zero;
			colorAttr.dstAlphaBlendFactor = BlendFactor::Zero;
			colorAttr.alphaBlendOp = BlendOperator::Add;

			initializer.colorBlendAttachments.push_back(colorAttr);

			initializer.isScissorDynamic = true;
			initializer.isViewportDynamic = true;

			initializer.depthStencil.depthTestEnable = true;
			initializer.depthStencil.depthBoundsTestEnable = false;
			initializer.depthStencil.depthWriteEnable = true;
			initializer.depthStencil.depthCompareOperator = CompareOperator::Less;
			initializer.depthStencil.minDepthBounds = 0;
			initializer.depthStencil.maxDepthBounds = 0;

			initializer.depthStencil.stencilTestEnable = false;
			StencilOperatorState stencilOpState;
			stencilOpState.failOperator = StencilOperator::Keep;
			stencilOpState.passOperator = StencilOperator::Keep;
			stencilOpState.compareOperator = CompareOperator::Always;
			stencilOpState.compareMask = 0;
			stencilOpState.reference = 0;
			stencilOpState.depthFailOperator = StencilOperator::Keep;
			stencilOpState.writeMask = 0;

			initializer.depthStencil.front = stencilOpState;
			initializer.depthStencil.back = stencilOpState;

			auto shaders = material->GetShaders();
			for(auto& shader : shaders) {
				initializer.shaders.push_back(shader->GetRenderShader());
			}

			initializer.descSetLayouts.push_back(mGlobalDescriptorSetLayout);
			initializer.descSetLayouts.push_back(material->GetDescriptorSetLayout());
			initializer.descSetLayouts.push_back(mPerObjectDescriptorSetLayout);

			initializer.renderPass = mRenderPass;

			return mRenderAPI->CreateGraphicsPipeline(initializer);
		}
	} // namespace core
} // namespace cube
