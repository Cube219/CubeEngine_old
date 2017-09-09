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
			mDepthBufferImage = mRenderAPI->CreateImage(ImageType::Image2D, DataFormat::D16_Unorm,
				platform->GetWindowWidth(), platform->GetWindowHeight(), 1, 1, ImageUsageBits::DepthStencilAttachmentBit);
			mDepthBufferImageView = mDepthBufferImage->GetImageView(DataFormat::D16_Unorm, ImageAspectBits::Depth, ImageViewType::Image2D);

			// Swapchain
			mSwapchain = mRenderAPI->CreateSwapchain();
			mSwapchain->Recreate(2, platform->GetWindowWidth(), platform->GetWindowHeight(), true);

			// RenderPass
			mRenderPass = mRenderAPI->CreateRenderPass();
			Color c;
			c.float32 = {0.3f, 0.3f, 0.3f, 0};
			mRenderPass->SetSwapchain(mSwapchain, LoadOperator::Clear, StoreOperator::Store, c, ImageLayout::Undefined, ImageLayout::PresentSource);
			
			c.float32 = {0, 0, 0, 0};
			DepthStencilValue v;
			v.depth = 1.0f;
			v.stencil = 0;
			mRenderPass->AddAttachment(mDepthBufferImageView, DataFormat::D16_Unorm, true,
				LoadOperator::Clear, StoreOperator::DontCare, LoadOperator::DontCare, StoreOperator::DontCare,
				c, ImageLayout::Undefined, ImageLayout::DepthStencilAttachmentOptimal, v);

			BaseRenderSubpass subpass;
			subpass.mColors.push_back({0, ImageLayout::ColorAttachmentOptimal});
			subpass.mDepthStencil.index = 1;
			subpass.mDepthStencil.layout = ImageLayout::DepthStencilAttachmentOptimal;
			mRenderPass->AddSubpass(subpass);

			mRenderPass->Create();

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
				"layout (location = 0) out vec4 outColor;\n"
				"void main(void) {\n"
				"   outColor = inColor;\n"
				"   gl_Position = myBufferVals.mvp * pos;\n"
				"}\n";

			String fragShaderText =
				"#version 440\n"
				"layout (location = 0) in vec4 color;\n"
				"layout (location = 0) out vec4 outColor;\n"
				"void main(void) {\n"
				"   outColor = color;\n"
				"}\n";

			mShaders.push_back(mRenderAPI->CreateShader(ShaderType::GLSL_Vertex, vertShaderText, String("main")));
			mShaders.push_back(mRenderAPI->CreateShader(ShaderType::GLSL_Fragment, fragShaderText, String("main")));

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

			mDescriptorSets.push_back(r3d->GetDescriptorSet());

			RecreatePipeline();

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

			mSwapchain->AcquireNextImageIndex(mGetImageSemaphore);

			RewriteCommandBuffer(); // TODO: 이걸 필요할 때만 쓰도록

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
			mDepthBufferImage = mRenderAPI->CreateImage(ImageType::Image2D, DataFormat::D16_Unorm,
				width, height, 1, 1, ImageUsageBits::DepthStencilAttachmentBit);
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

			for(auto r : mRenderers) {
				r->Draw(mMainCommandBuffer, mCameraRenderer);
			}

			mMainCommandBuffer->End();
		}

		void RendererManager::RecreatePipeline()
		{
			// Get a graphics pipeline
			mGraphicsPipeline = mRenderAPI->CreateGraphicsPipeline();
			// TODO: Vertex 구조체를 기반으로 다시 쓰기(sizeof...)
			mGraphicsPipeline->AddVertexInputAttribute(0, DataFormat::R32G32B32A32_SFloat, 0);
			mGraphicsPipeline->AddVertexInputAttribute(1, DataFormat::R32G32B32A32_SFloat, 16);
			mGraphicsPipeline->SetVertexInput(sizeof(Vertex));
			mGraphicsPipeline->SetVertexTopology(VertexTopology::Triangle);

			mGraphicsPipeline->SetRasterizer(PolygonMode::Fill, PolygonFrontFace::Clockwise, CullMode::Back);

			mGraphicsPipeline->AddColorBlendAttachment(false,
				BlendFactor::Zero, BlendFactor::Zero, BlendOperator::Add,
				BlendFactor::Zero, BlendFactor::Zero, BlendOperator::Add);
			mGraphicsPipeline->SetColorBlend(false, LogicOperator::NoOp, 1.0f, 1.0f, 1.0f, 1.0f);

			mGraphicsPipeline->AddViewport({}, true);
			mGraphicsPipeline->AddScissor({}, true);
			mGraphicsPipeline->SetViewportState();

			StencilOperatorState stencilOpState;
			stencilOpState.failOperator = StencilOperator::Keep;
			stencilOpState.passOperator = StencilOperator::Keep;
			stencilOpState.compareOperator = CompareOperator::Always;
			stencilOpState.compareMask = 0;
			stencilOpState.reference = 0;
			stencilOpState.depthFailOperator = StencilOperator::Keep;
			stencilOpState.writeMask = 0;
			mGraphicsPipeline->SetDepthStencil(true, false, true, CompareOperator::Always, false, stencilOpState, stencilOpState, 0, 0);

			mGraphicsPipeline->SetMultisampler();

			for(auto& shader : mShaders) {
				mGraphicsPipeline->AddShader(shader);
			}

			for(auto& desc : mDescriptorSets) {
				mGraphicsPipeline->AddDescriptorSet(desc);
			}

			mGraphicsPipeline->Create(mRenderPass);
		}
	}
}