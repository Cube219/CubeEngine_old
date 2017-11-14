#pragma once

#include "..\EngineCoreHeader.h"

#include "BasePlatform\BasePlatform.h"
#include "BasePlatform\BasePlatformDLib.h"

#include "BaseRenderAPI\BaseRenderAPI.h"
#include "BaseRenderAPI\Wrapper\BaseRenderShader.h"
#include "BaseRenderAPI\Wrapper\BaseRenderGraphicsPipeline.h"
#include "BaseRenderAPI\Wrapper\BaseRenderImage.h"
#include "BaseRenderAPI\Wrapper\BaseRenderSwapchain.h"
#include "BaseRenderAPI\Wrapper\BaseRenderCommandBuffer.h"
#include "BaseRenderAPI\Wrapper\BaseRenderQueue.h"
#include "BaseRenderAPI\Wrapper\BaseRenderSemaphore.h"
#include "BaseRenderAPI\Wrapper\BaseRenderFence.h"
#include "BaseRenderAPI\Wrapper\BaseRenderRenderPass.h"

namespace cube
{
	namespace core
	{
		enum class RenderType
		{
			Vulkan
		};

		class RendererManager
		{
		public:
			RendererManager(SPtr<platform::BasePlatform>& platform, RenderType type);
			~RendererManager();

			SPtr<Renderer3D> CreateRenderer3D();
			SPtr<CameraRenderer3D> GetCameraRenderer3D();

			SPtr<BaseRenderAPI> GetRenderAPI() const { return mRenderAPI; }

			void DrawAll();

			void Resize(uint32_t width, uint32_t height);

			void SetVsync(bool vsync);

		private:
			void RewriteCommandBuffer();
			void RecreatePipeline();

			SPtr<platform::BasePlatformDLib> mRenderDLib;
			SPtr<BaseRenderAPI> mRenderAPI;

			Vector<SPtr<Renderer3D>> mRenderers;
			Vector<SPtr<BaseRenderDescriptorSet>> mDescriptorSets;
			SPtr<CameraRenderer3D> mCameraRenderer;

			SPtr<BaseRenderImage> mDepthBufferImage;
			SPtr<BaseRenderImageView> mDepthBufferImageView;

			SPtr<BaseRenderSwapchain> mSwapchain;

			SPtr<BaseRenderRenderPass> mRenderPass;

			Vector<SPtr<BaseRenderShader>> mShaders;

			SPtr<BaseRenderGraphicsPipeline> mGraphicsPipeline;

			SPtr<BaseRenderCommandBuffer> mMainCommandBuffer;
			SPtr<BaseRenderFence> mMainCommandBufferSubmitFence;

			SPtr<BaseRenderQueue> mGraphicsQueue;

			SPtr<BaseRenderSemaphore> mGetImageSemaphore;

			bool mIsPrepared;

			bool mVsync;
			uint32_t mWidth;
			uint32_t mHeight;

			SPtr<platform::BasePlatform> mPlatform_ref;
		};
	}
}