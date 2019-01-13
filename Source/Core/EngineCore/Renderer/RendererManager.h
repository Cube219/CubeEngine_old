#pragma once

#include "../EngineCoreHeader.h"

#include <thread>

#include "DLib.h"

/*
[old]
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
*/
#include "BaseRenderAPI_New/RenderAPI.h"
#include "BaseRenderAPI_New/Interface/Device.h"
#include "BaseRenderAPI_New/Interface/Texture.h"
#include "BaseRenderAPI_New/Interface/SwapChain.h"
#include "BaseRenderAPI_New/Interface/RenderTarget.h"
#include "BaseRenderAPI_New/Interface/RenderPass.h"
#include "BaseRenderAPI_New/Interface/CommandList.h"
#include "BaseRenderAPI_New/Interface/ShaderParametersLayout.h"
#include "BaseRenderAPI_New/Interface/ShaderParameters.h"
#include "BaseRenderAPI_New/Interface/Fence.h"
#include "BaseRenderAPI_New/Interface/GraphicsPipelineState.h"

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
			RendererManager() : 
				mIsPrepared(false), mDirLight(nullptr)
			{}
			~RendererManager() {}

			RendererManager(const RendererManager& other) = delete;
			RendererManager& operator=(const RendererManager& rhs) = delete;
			RendererManager(RendererManager&& other) = delete;
			RendererManager& operator=(RendererManager&& rhs) = delete;

			void Initialize(RenderType type);
			void ShutDown();

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
			SPtr<render::Device> GetDevice() const { return mDevice; }

			void DrawAll();

			void Resize(uint32_t width, uint32_t height);

			void SetVsync(bool vsync);

			SPtr<render::ShaderParametersLayout> _GetPerObjectShaderParametersLayout(){ return mPerObjectShaderParametersLayout; }

		private:
			friend class EngineCore;

			void CreateDepthBuffer();
			void CreateRenderpass();

			void RewriteCommandBuffer();
			void DrawRenderer3D(Uint32 commandListIndex, SPtr<Renderer3D_RT>& renderer);

			SPtr<render::GraphicsPipelineState> CreatePipeline(SPtr<Material_RT> material);

			SPtr<platform::DLib> mRenderDLib;
			//[old] SPtr<render::RenderAPI> mRenderAPI;
			SPtr<render::RenderAPI> mRenderAPI;
			SPtr<render::Device> mDevice;

			Mutex mRenderersMutex;
			Vector<SPtr<Renderer3D_RT>> mRenderers;
			SPtr<CameraRenderer3D_RT> mCameraRenderer;
			SPtr<CameraRenderer3D> mCameraRenderer_NotRT;

			Mutex mMaterialsMutex;
			Vector<SPtr<Material_RT>> mMaterials;
			//[old] Vector<SPtr<render::GraphicsPipeline>> mMaterialPipelines;
			Vector <SPtr<render::GraphicsPipelineState>> mMaterialPipelines;

			SPtr<DirectionalLight_RT> mDirLight;
			//[old] SPtr<render::Buffer> mDirLightBuffer;

			Mutex mPointLightsMutex;
			Vector<SPtr<PointLight_RT>> mPointLights;
			//[old] SPtr<render::Buffer> mPointLightsBuffer;

			//[old] SPtr<render::DescriptorSetLayout> mGlobalDescriptorSetLayout;
			//[old] SPtr<render::DescriptorSet> mGlobalDescriptorSet;
			//[old] SPtr<render::Buffer> mGlobalUBOBuffer;
			//[old] SPtr<render::DescriptorSetLayout> mPerObjectDescriptorSetLayout;
			SPtr<render::ShaderParametersLayout> mGlobalShaderParametersLayout;
			SPtr<render::ShaderParameters> mGlobalShaderParameters;
			SPtr<render::ShaderParametersLayout> mPerObjectShaderParametersLayout;

			//[old] SPtr<render::Image> mDepthBufferImage;
			//[old] SPtr<render::ImageView> mDepthBufferImageView;
			SPtr<render::Texture> mDepthBufferTexture;
			SPtr<render::TextureView> mDepthBufferTextureView;

			//[old] SPtr<render::Swapchain> mSwapchain;
			SPtr<render::SwapChain> mSwapChain;

			//[old] SPtr<render::RenderPass> mRenderPass;
			SPtr<render::RenderTarget> mColorRenderTarget;
			SPtr<render::RenderTarget> mDepthStencilRenderTarget;
			SPtr<render::RenderPass> mRenderPass;

			//[old] Vector<SPtr<render::CommandBuffer>> mCommandBuffers;
			//[old] Vector<int> mCommandBuffersCurrentMaterialIndex;
			//[old] SPtr<render::CommandBuffer> mMainCommandBuffer;
			//[old] SPtr<render::Fence> mMainCommandBufferSubmitFence;
			Vector<SPtr<render::CommandList>> mCommandLists;
			Vector<int> mCommandListsCurrentMaterialIndex;
			SPtr<render::CommandList> mMainCommandList;

			//[old] SPtr<render::Queue> mGraphicsQueue;

			//[old] SPtr<render::Semaphore> mGetImageSemaphore;

			bool mIsPrepared;

			bool mVsync;
			Uint32 mWidth;
			Uint32 mHeight;

			Uint32 mNumThreads;
		};
	} // namespace core
} // namespace cube
