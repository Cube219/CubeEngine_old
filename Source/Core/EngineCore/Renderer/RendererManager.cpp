#include "RendererManager.h"

#include "Platform.h"
#include "../LogWriter.h"
#include "Renderer3D.h"
#include "CameraRenderer3D.h"
#include "Material/Shader.h"
#include "Material/Material.h"
#include "Material/MaterialInstance.h"

#include <fstream>
#include <sstream>

namespace cube
{
	namespace core
	{
		RendererManager::RendererManager(RenderType type) :
			mIsPrepared(false)
		{
			mWidth = platform::Platform::GetWindowWidth();
			mHeight = platform::Platform::GetWindowHeight();

			switch(type) {
				case RenderType::Vulkan:
					mRenderDLib = platform::Platform::LoadDLib(CUBE_T("VulkanAPI"));
					break;

				default:
					CUBE_LOG(LogType::Error, "Unknown renderer type");
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

			mGetImageSemaphore = mRenderAPI->CreateSemaphore();

			// Create camera renderer
			// TODO: multiple camera
			mCameraRenderer = std::make_shared<CameraRenderer3D>();

			// Create Global / mPerObjectDescriptorSetLayout
			render::DescriptorSetInitializer descSetInit;

			mGlobalDescriptorSetLayout = mRenderAPI->CreateDescriptorSetLayout(descSetInit);
			mGlobalDescriptorSet = mRenderAPI->CreateDescriptorSet(mGlobalDescriptorSetLayout);

			descSetInit.descriptors.clear();
			descSetInit.descriptors.push_back({render::ShaderTypeBits::Vertex, render::DescriptorType::UniformBuffer, 0, 1});
			mPerObjectDescriptorSetLayout = mRenderAPI->CreateDescriptorSetLayout(descSetInit);

			mIsPrepared = true;
		}

		RendererManager::~RendererManager()
		{
			mRenderers.clear();
		}

		HMaterial RendererManager::RegisterMaterial(SPtr<Material>& material)
		{
			Lock(mMaterialsMutex);

			material->mIndex = (int)mMaterials.size();

			SPtr<MaterialData> matDataPtr = std::make_shared<MaterialData>();
			matDataPtr->data = std::move(material);
			matDataPtr->data->mMyHandler = HMaterial(matDataPtr);
			mMaterials.push_back(matDataPtr);
			
			HMaterial hMat = HMaterial(matDataPtr);
			mMaterialPipelines.push_back(CreatePipeline(hMat));
			mMaterialCommandBuffers.push_back(mRenderAPI->CreateCommandBuffer(false));

			return hMat;
		}

		void RendererManager::UnregisterMaterial(HMaterial& material)
		{
			int index = material->mIndex;

			if(index == -1) {
				CUBE_LOG(LogType::Error, "This material is not registed.");
				return;
			}

			Lock(mMaterialsMutex);

			int lastIndex = mMaterials.back()->data->mIndex;
			mMaterials[lastIndex]->data->mIndex = index;
			
			mMaterials[index]->data = nullptr;

			std::swap(mMaterials[index], mMaterials[lastIndex]);
			std::swap(mMaterialPipelines[index], mMaterialPipelines[lastIndex]);
			std::swap(mMaterialCommandBuffers[index], mMaterialCommandBuffers[lastIndex]);
			mMaterials.pop_back();
			mMaterialPipelines.pop_back();
			mMaterialCommandBuffers.pop_back();
		}

		void RendererManager::RegisterRenderer3D(SPtr<Renderer3D>& renderer)
		{
			if(renderer->mIndex != -1)
				return;

			Lock(mRenderersMutex);

			mRenderers.push_back(renderer);
			renderer->mIndex = (int)mRenderers.size() - 1;
		}

		void RendererManager::UnregisterRenderer3D(SPtr<Renderer3D>& renderer)
		{
			int index = renderer->mIndex;

			if(index == -1) {
				CUBE_LOG(LogType::Error, "This renderer is not registed.");
				return;
			}

			Lock(mRenderersMutex);

			int lastIndex = mRenderers.back()->mIndex;
			mRenderers[lastIndex]->mIndex = index;

			std::swap(mRenderers[index], mRenderers[lastIndex]);
			mRenderers.pop_back();

			renderer->mIndex = -1;
		}

		SPtr<Renderer3D> RendererManager::CreateRenderer3D()
		{
			return std::make_shared<Renderer3D>(mRenderAPI, mPerObjectDescriptorSetLayout);
		}

		SPtr<CameraRenderer3D> RendererManager::GetCameraRenderer3D()
		{
			return mCameraRenderer;
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
			if(r == true)
				mSwapchain->Present(0, nullptr);
			else {
				CUBE_LOG(LogType::Error, "Main command buffer submit fence time out");
			}
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
			c.float32 = {0.3f, 0.3f, 0.3f, 0};
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

			// Prepare all command buffers of each material
			for(uint32_t i = 0; i < mMaterialCommandBuffers.size(); i++) {
				mMaterialCommandBuffers[i]->Reset();
				mMaterialCommandBuffers[i]->Begin();

				mMaterialCommandBuffers[i]->SetRenderPass(mRenderPass, renderArea);

				mMaterialCommandBuffers[i]->BindGraphicsPipeline(mMaterialPipelines[i]);

				mMaterialCommandBuffers[i]->SetViewport(0, 1, &vp);
				mMaterialCommandBuffers[i]->SetScissor(0, 1, &scissor);

				//mMaterialCommandBuffers[i]->BindDescriptorSets(PipelineType::Graphics, 0, 1, &mGlobalDescriptorSet);
			}

			// TODO: MaterialInstance를 먼저 찾고 그것에 등록된 renderer들을 찾는 방식으로
			//       그렇게하면 BindDescriptorSets 횟수를 줄일 수 있음
			//       그럴러면 Material에서 Instance를 생성할 때 저장해둬야 함
			for(auto& renderer : mRenderers) {
				HMaterialInstance materialIns = renderer->GetMaterialInstance();
				int materialIndex = materialIns->GetMaterial()->mIndex;
				SPtr<render::DescriptorSet> materialInsDesc = materialIns->GetDescriptorSet();

				mMaterialCommandBuffers[materialIndex]->BindDescriptorSets(PipelineType::Graphics, 0, 1, &materialInsDesc);

				renderer->Draw(mMaterialCommandBuffers[materialIndex], mCameraRenderer);
			}

			for(auto& cmd : mMaterialCommandBuffers) {
				cmd->End();
			}

			// Main command buffer
			mMainCommandBuffer->Reset();

			mMainCommandBuffer->Begin();

			mMainCommandBuffer->SetRenderPass(mRenderPass, renderArea);

			mMainCommandBuffer->ExecuteCommands((uint32_t)mMaterialCommandBuffers.size(), mMaterialCommandBuffers.data());

			mMainCommandBuffer->End();
		}

		SPtr<render::GraphicsPipeline> RendererManager::CreatePipeline(HMaterial& material)
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

			//initializer.descSetLayouts.push_back(mGlobalDescriptorSetLayout);
			initializer.descSetLayouts.push_back(material->GetDescriptorSetLayout());
			initializer.descSetLayouts.push_back(mPerObjectDescriptorSetLayout);

			initializer.renderPass = mRenderPass;

			return mRenderAPI->CreateGraphicsPipeline(initializer);
		}
	} // namespace core
} // namespace cube
