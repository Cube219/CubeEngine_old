#include "RendererManager.h"

#include "Renderer3D.h"
#include "CameraRenderer3D.h"

namespace cube
{
	namespace core
	{
		RendererManager::RendererManager(SPtr<platform::BasePlatform>& platform, RenderType type) :
			mPlatform_ref(platform), mIsPrepared(false)
		{
			mWidth = platform->GetWindowWidth();
			mHeight = platform->GetWindowHeight();

			switch(type) {
				case RenderType::Vulkan:
					mRenderDLib = platform->LoadDLib(L"VulkanAPI");
					break;

				default:
					LogWriter::WriteLog(L"Unknown renderer type");
					return;
			}

			using CreateAPIFunction = BaseRenderAPI*(*)();

			auto createAPIFunction = RCast(CreateAPIFunction)(mRenderDLib->GetFunction("CreateAPI"));

			// TODO: 좀 더 좋은 방법은 없을까?
			SPtr<BaseRenderAPI> temp(createAPIFunction());
			mRenderAPI = std::move(temp);

			mRenderAPI->Init(platform);

			// Graphics queue
			mGraphicsQueue = mRenderAPI->GetQueue(QueueTypeBits::GraphicsBit, 0);

			// Depth buffer
			BaseRenderImageInitializer imageInit;
			imageInit.type = ImageType::Image2D;
			imageInit.format = DataFormat::D16_Unorm;
			imageInit.width = platform->GetWindowWidth();
			imageInit.height = platform->GetWindowHeight();
			imageInit.depth = 1;
			imageInit.mipLevels = 1;
			imageInit.usage = ImageUsageBits::DepthStencilAttachmentBit;
			mDepthBufferImage = mRenderAPI->CreateImage(imageInit);
			mDepthBufferImageView = mDepthBufferImage->GetImageView(DataFormat::D16_Unorm, ImageAspectBits::Depth, ImageViewType::Image2D);

			// Swapchain
			mSwapchain = mRenderAPI->CreateSwapchain();
			mSwapchain->Recreate(2, platform->GetWindowWidth(), platform->GetWindowHeight(), true);

			// RenderPass
			Color c;
			DepthStencilValue v;

			BaseRenderRenderPassInitializer renderPassInit;
			// Depth buffer attachment
			BaseRenderRenderPassInitializer::Attachment att;
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
			BaseRenderRenderPassInitializer::SwapchainAttachment swapAtt;
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
			BaseRenderSubpass subpass;
			subpass.mColors.push_back({1, ImageLayout::ColorAttachmentOptimal});
			subpass.mDepthStencil.index = 0;
			subpass.mDepthStencil.layout = ImageLayout::DepthStencilAttachmentOptimal;
			renderPassInit.subpasses.push_back(subpass);

			mRenderPass = mRenderAPI->CreateRenderPass(renderPassInit);

			// Get a vertex / fragment shader
			String vertShaderText =
				"#version 440\n"
				//"#extension GL_ARB_separate_shader_objects : enable\n"
				//"#extension GL_ARB_shading_language_420pack : enable\n"
				"layout (binding = 0) uniform bufferVals {\n"
				"    mat4 mvp;\n"
				"} myBufferVals;\n"
				"layout (location = 0) in vec4 pos;\n"
				"layout (location = 1) in vec4 inColor;\n"
				"layout (location = 2) in vec2 inTexCoord;\n"
				"layout (location = 0) out vec4 outColor;\n"
				"layout (location = 1) out vec2 outTexCoord;\n"
				"void main(void) {\n"
				"   outColor = inColor;\n"
				"   outTexCoord = inTexCoord;\n"
				"   gl_Position = myBufferVals.mvp * pos;\n"
				"}\n";

			String fragShaderText =
				"#version 440\n"
				"layout (binding = 1) uniform sampler2D texSampler;\n"
				"layout (location = 0) in vec4 color;\n"
				"layout (location = 1) in vec2 texCoord;\n"
				"layout (location = 0) out vec4 outColor;\n"
				"void main(void) {\n"
				"   outColor = texture(texSampler, texCoord);\n"
				"}\n";

			BaseRenderShaderInitializer shaderInit;
			shaderInit.type = ShaderType::GLSL_Vertex;
			shaderInit.code = vertShaderText.c_str();
			shaderInit.entryPoint = "main";
			mShaders.push_back(mRenderAPI->CreateShader(shaderInit));

			shaderInit.type = ShaderType::GLSL_Fragment;
			shaderInit.code = fragShaderText.c_str();
			shaderInit.entryPoint = "main";
			mShaders.push_back(mRenderAPI->CreateShader(shaderInit));

			// Get a main command buffer
			mMainCommandBuffer = mRenderAPI->CreateCommandBuffer();
			mMainCommandBufferSubmitFence = mRenderAPI->CreateFence();

			mGetImageSemaphore = mRenderAPI->CreateSemaphore();

			// Create camera renderer
			// TODO: multiple camera
			mCameraRenderer = std::make_shared<CameraRenderer3D>();

			mIsPrepared = true;
		}

		RendererManager::~RendererManager()
		{
			mRenderers.clear();
		}

		SPtr<Renderer3D> RendererManager::CreateRenderer3D()
		{
			SPtr<Renderer3D> r3d = std::make_shared<Renderer3D>(mRenderAPI);
			mRenderers.push_back(r3d);

			mIsPipelineDirty = true;

			return r3d;
		}

		SPtr<CameraRenderer3D> RendererManager::GetCameraRenderer3D()
		{
			return mCameraRenderer;
		}

		void RendererManager::DrawAll()
		{
			if(mIsPrepared == false)
				return;

			if(mIsPipelineDirty == true) {
				RecreatePipeline();

				mIsPipelineDirty = false;
			}

			mSwapchain->AcquireNextImageIndex(mGetImageSemaphore);

			RewriteCommandBuffer(); // TODO: 이걸 필요할 때만 쓰도록

			mMainCommandBufferSubmitFence->Reset();
			auto temp = std::make_pair(mGetImageSemaphore, PipelineStageBits::ColorAttachmentOutputBit);
			mMainCommandBuffer->Submit(mGraphicsQueue, 1, &temp, 0, nullptr, mMainCommandBufferSubmitFence);
			// TODO: 예외처리
			bool r = mMainCommandBufferSubmitFence->Wait(100000000);

			if(r == true)
				mSwapchain->Present(0, nullptr);
		}

		void RendererManager::Resize(uint32_t width, uint32_t height)
		{
			mIsPrepared = false;

			mWidth = width;
			mHeight = height;

			// Recreate a depth buffer
			BaseRenderImageInitializer imageInit;
			imageInit.type = ImageType::Image2D;
			imageInit.format = DataFormat::D16_Unorm;
			imageInit.width = width;
			imageInit.height = height;
			imageInit.depth = 1;
			imageInit.mipLevels = 1;
			imageInit.usage = ImageUsageBits::DepthStencilAttachmentBit;
			mDepthBufferImage = mRenderAPI->CreateImage(imageInit);
			mDepthBufferImageView = mDepthBufferImage->GetImageView(DataFormat::D16_Unorm, ImageAspectBits::Depth, ImageViewType::Image2D);

			mSwapchain->Recreate(2, width, height, mVsync);

			mIsPrepared = true;
		}

		void RendererManager::SetVsync(bool vsync)
		{
			mVsync = vsync;

			mSwapchain->Recreate(2, mWidth, mHeight, vsync);
		}

		void RendererManager::RewriteCommandBuffer()
		{
			mMainCommandBuffer->Reset();

			mMainCommandBuffer->Begin();

			Rect2D r;
			r.x = 0;
			r.y = 0;
			r.width = mWidth;
			r.height = mHeight;
			mMainCommandBuffer->SetRenderPass(mRenderPass, r);

			Viewport vp;
			vp.width = SCast(float)(mWidth);
			vp.height = SCast(float)(mHeight);
			vp.minDepth = 0.0f;
			vp.maxDepth = 1.0f;
			vp.x = 0;
			vp.y = 0;
			mMainCommandBuffer->SetViewport(0, 1, &vp);
			Rect2D scissor;
			scissor.x = 0;
			scissor.y = 0;
			scissor.width = mWidth;
			scissor.height = mHeight;
			mMainCommandBuffer->SetScissor(0, 1, &scissor);

			mMainCommandBuffer->BindGraphicsPipeline(mGraphicsPipeline);

			for(auto& r : mRenderers) {
				r->Draw(mMainCommandBuffer, mCameraRenderer);
			}

			mMainCommandBuffer->End();
		}

		void RendererManager::RecreatePipeline()
		{
			BaseRenderGraphicsPipelineInitializer initializer;

			BaseRenderGraphicsPipelineInitializer::VertexInputAttribute attr;
			attr.location = 0;
			attr.format = DataFormat::R32G32B32A32_SFloat;
			attr.offset = 0;
			initializer.vertexInputAttributes.push_back(attr);
			
			attr.location = 1;
			attr.format = DataFormat::R32G32B32A32_SFloat;
			attr.offset = 16;
			initializer.vertexInputAttributes.push_back(attr);
			
			attr.location = 2;
			attr.format = DataFormat::R32G32_SFloat;
			attr.offset = 32;
			initializer.vertexInputAttributes.push_back(attr);

			initializer.vertexSize = sizeof(Vertex);
			initializer.vertexTopology = VertexTopology::Triangle;

			initializer.rasterizer = {PolygonMode::Fill, PolygonFrontFace::Clockwise, CullMode::Back};

			BaseRenderGraphicsPipelineInitializer::ColorBlendAttachment colorAttr;
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
			initializer.depthStencil.depthCompareOperator = CompareOperator::Always;
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

			for(auto& shader : mShaders) {
				initializer.shaders.push_back(shader);
			}

			for(auto& r3d : mRenderers) {
				initializer.descSets.push_back(r3d->GetDescriptorSet());
			}

			initializer.renderPass = mRenderPass;

			mGraphicsPipeline = mRenderAPI->CreateGraphicsPipeline(initializer);
		}
	}
}