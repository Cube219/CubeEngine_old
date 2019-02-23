#pragma once

#include "../EngineCoreHeader.h"

#include <thread>

#include "DLib.h"

#include "RenderAPI/RenderAPI.h"
#include "RenderAPI/Interface/Device.h"
#include "RenderAPI/Interface/Texture.h"
#include "RenderAPI/Interface/SwapChain.h"
#include "RenderAPI/Interface/RenderTarget.h"
#include "RenderAPI/Interface/RenderPass.h"
#include "RenderAPI/Interface/CommandList.h"
#include "RenderAPI/Interface/ShaderParametersLayout.h"
#include "RenderAPI/Interface/ShaderParameters.h"
#include "RenderAPI/Interface/Fence.h"
#include "RenderAPI/Interface/GraphicsPipelineState.h"

#include "../Thread/MutexLock.h"

namespace cube
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

		void StartFrame();
		void DrawAll();

		void Resize(Uint32 width, Uint32 height);

		void SetVsync(bool vsync);

		SPtr<render::ShaderParametersLayout> _GetPerObjectShaderParametersLayout(){ return mPerObjectShaderParametersLayout; }

	private:
		friend class EngineCore;

		void CreateDepthBuffer();
		void CreateRenderpass();

		void RewriteCommandBuffer();
		void DrawRenderer3D(Uint32 commandListIndex, SPtr<rt::Renderer3D>& renderer);

		SPtr<render::GraphicsPipelineState> CreatePipeline(SPtr<rt::Material> material);

		SPtr<platform::DLib> mRenderDLib;
		SPtr<render::RenderAPI> mRenderAPI;
		SPtr<render::Device> mDevice;

		Mutex mRenderersMutex;
		Vector<SPtr<rt::Renderer3D>> mRenderers;
		SPtr<rt::CameraRenderer3D> mCameraRenderer;
		SPtr<CameraRenderer3D> mCameraRenderer_NotRT;

		Mutex mMaterialsMutex;
		Vector<SPtr<rt::Material>> mMaterials;
		Vector <SPtr<render::GraphicsPipelineState>> mMaterialPipelines;

		SPtr<rt::DirectionalLight> mDirLight;

		Mutex mPointLightsMutex;
		Vector<SPtr<rt::PointLight>> mPointLights;

		SPtr<render::ShaderParametersLayout> mGlobalShaderParametersLayout;
		SPtr<render::ShaderParameters> mGlobalShaderParameters;
		SPtr<render::ShaderParametersLayout> mPerObjectShaderParametersLayout;

		SPtr<render::Texture> mDepthBufferTexture;
		SPtr<render::TextureView> mDepthBufferTextureView;

		SPtr<render::SwapChain> mSwapChain;

		SPtr<render::RenderTarget> mColorRenderTarget;
		SPtr<render::RenderTarget> mDepthStencilRenderTarget;
		SPtr<render::RenderPass> mRenderPass;

		Vector<SPtr<render::CommandList>> mCommandLists;
		Vector<int> mCommandListsCurrentMaterialIndex;
		SPtr<render::CommandList> mMainCommandList;

		bool mIsPrepared;

		bool mVsync;
		Uint32 mWidth;
		Uint32 mHeight;

		Uint32 mNumThreads;
	};
} // namespace cube
