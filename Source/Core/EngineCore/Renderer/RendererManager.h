#pragma once

#include "../EngineCoreHeader.h"

#include <thread>

#include "DLib.h"

#include "BaseRenderAPI/RenderAPI.h"
#include "BaseRenderAPI/Wrapper/Shader.h"
#include "BaseRenderAPI/Wrapper/GraphicsPipeline.h"
#include "BaseRenderAPI/Wrapper/Image.h"
#include "BaseRenderAPI/Wrapper/Swapchain.h"
#include "BaseRenderAPI/Wrapper/CommandBuffer.h"
#include "BaseRenderAPI/Wrapper/Queue.h"
#include "BaseRenderAPI/Wrapper/Semaphore.h"
#include "BaseRenderAPI/Wrapper/Fence.h"
#include "BaseRenderAPI/Wrapper/RenderPass.h"

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
			constexpr static int maxPointLightNum = 10;

		public:
			RendererManager(EngineCore* eCore);
			~RendererManager();

			void Prepare(RenderType type);

			HMaterial RegisterMaterial(SPtr<Material>& material);
			void UnregisterMaterial(HMaterial& material);

			void RegisterRenderer3D(SPtr<Renderer3D>& renderer);
			void UnregisterRenderer3D(SPtr<Renderer3D>& renderer);

			void RegisterLight(SPtr<DirectionalLight>& dirLight);
			void UnregisterLight(SPtr<DirectionalLight>& dirLight);
			void RegisterLight(SPtr<PointLight>& pointLight);
			void UnregisterLight(SPtr<PointLight>& pointLight);

			SPtr<Renderer3D> CreateRenderer3D();
			SPtr<CameraRenderer3D> GetCameraRenderer3D(); // TODO: 차후 저렇게 바꾸기

			SPtr<render::RenderAPI> GetRenderAPI() const { return mRenderAPI; }

			void DrawAll();

			void Resize(uint32_t width, uint32_t height);

			void SetVsync(bool vsync);

		private:
			friend class EngineCore;

			void CreateDepthBuffer();
			void CreateRenderpass();

			void RewriteCommandBuffer();
			void DrawRenderer3D(uint32_t commandBufferIndex, SPtr<Renderer3D>& renderer);

			SPtr<render::GraphicsPipeline> CreatePipeline(HMaterial& material);

			EngineCore* mECore;
			std::thread mGameThread;

			ThreadNotify mFinishPreparingNotify;
			ThreadNotify mFinishRenderingNotify;
			ThreadNotify mFinishProcessingTaskBufferNotify;
			ThreadNotify mFinishTerminating;

			SPtr<platform::DLib> mRenderDLib;
			SPtr<render::RenderAPI> mRenderAPI;

			Mutex mRenderersMutex;
			Vector<SPtr<Renderer3D>> mRenderers;
			SPtr<CameraRenderer3D> mCameraRenderer;

			Mutex mMaterialsMutex;
			Vector<SPtr<MaterialData>> mMaterials;
			Vector<SPtr<render::GraphicsPipeline>> mMaterialPipelines;

			SPtr<DirectionalLight> mDirLight;
			SPtr<render::Buffer> mDirLightBuffer;

			Vector<SPtr<PointLight>> mPointLights;
			SPtr<render::Buffer> mPointLightsBuffer;

			SPtr<render::DescriptorSetLayout> mGlobalDescriptorSetLayout;
			SPtr<render::DescriptorSet> mGlobalDescriptorSet;
			SPtr<render::Buffer> mGlobalUBOBuffer;
			SPtr<render::DescriptorSetLayout> mPerObjectDescriptorSetLayout;

			SPtr<render::Image> mDepthBufferImage;
			SPtr<render::ImageView> mDepthBufferImageView;

			SPtr<render::Swapchain> mSwapchain;

			SPtr<render::RenderPass> mRenderPass;

			Vector<SPtr<render::CommandBuffer>> mCommandBuffers;
			Vector<int> mCommandBuffersCurrentMaterialIndex;
			SPtr<render::CommandBuffer> mMainCommandBuffer;
			SPtr<render::Fence> mMainCommandBufferSubmitFence;

			SPtr<render::Queue> mGraphicsQueue;

			SPtr<render::Semaphore> mGetImageSemaphore;

			bool mIsPrepared;

			bool mVsync;
			uint32_t mWidth;
			uint32_t mHeight;

			uint32_t mNumThreads;
		};
	} // namespace core
} // namespace cube
