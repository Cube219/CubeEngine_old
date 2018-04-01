#pragma once

#include "../EngineCoreHeader.h"

#include "DLib.h"

#include "BaseRenderAPI/BaseRenderAPI.h"
#include "BaseRenderAPI/Wrapper/BaseRenderShader.h"
#include "BaseRenderAPI/Wrapper/BaseRenderGraphicsPipeline.h"
#include "BaseRenderAPI/Wrapper/BaseRenderImage.h"
#include "BaseRenderAPI/Wrapper/BaseRenderSwapchain.h"
#include "BaseRenderAPI/Wrapper/BaseRenderCommandBuffer.h"
#include "BaseRenderAPI/Wrapper/BaseRenderQueue.h"
#include "BaseRenderAPI/Wrapper/BaseRenderSemaphore.h"
#include "BaseRenderAPI/Wrapper/BaseRenderFence.h"
#include "BaseRenderAPI/Wrapper/BaseRenderRenderPass.h"

#include "../Thread/MutexLock.h"

namespace cube
{
	namespace core
	{
		enum class RenderType
		{
			Vulkan
		};

		class ENGINE_CORE_EXPORT RendererManager
		{
		public:
			RendererManager(RenderType type);
			~RendererManager();

			HMaterial RegisterMaterial(UPtr<Material>& material);
			void UnregisterMaterial(HMaterial& material);

			void RegisterRenderer3D(SPtr<Renderer3D>& renderer);
			void UnregisterRenderer3D(SPtr<Renderer3D>& renderer);

			SPtr<Renderer3D> CreateRenderer3D();
			SPtr<CameraRenderer3D> GetCameraRenderer3D(); // TODO: 차후 저렇게 바꾸기

			SPtr<BaseRenderAPI> GetRenderAPI() const { return mRenderAPI; }

			void DrawAll();

			void Resize(uint32_t width, uint32_t height);

			void SetVsync(bool vsync);

		private:
			void CreateDepthBuffer();
			void CreateRenderpass();

			void RewriteCommandBuffer();

			SPtr<BaseRenderGraphicsPipeline> CreatePipeline(HMaterial& material);

			SPtr<platform::DLib> mRenderDLib;
			SPtr<BaseRenderAPI> mRenderAPI;

			Mutex mRenderersMutex;
			Vector<SPtr<Renderer3D>> mRenderers;
			SPtr<CameraRenderer3D> mCameraRenderer;

			Mutex mMaterialsMutex;
			Vector<SPtr<MaterialData>> mMaterials;
			Vector<SPtr<BaseRenderGraphicsPipeline>> mMaterialPipelines;
			Vector<SPtr<BaseRenderCommandBuffer>> mMaterialCommandBuffers;

			SPtr<BaseRenderDescriptorSetLayout> mGlobalDescriptorSetLayout;
			SPtr<BaseRenderDescriptorSet> mGlobalDescriptorSet;
			SPtr<BaseRenderDescriptorSetLayout> mPerObjectDescriptorSetLayout;

			SPtr<BaseRenderImage> mDepthBufferImage;
			SPtr<BaseRenderImageView> mDepthBufferImageView;

			SPtr<BaseRenderSwapchain> mSwapchain;

			SPtr<BaseRenderRenderPass> mRenderPass;

			SPtr<BaseRenderCommandBuffer> mMainCommandBuffer;
			SPtr<BaseRenderFence> mMainCommandBufferSubmitFence;

			SPtr<BaseRenderQueue> mGraphicsQueue;

			SPtr<BaseRenderSemaphore> mGetImageSemaphore;

			bool mIsPrepared;

			bool mVsync;
			uint32_t mWidth;
			uint32_t mHeight;
		};
	} // namespace core
} // namespace cube
