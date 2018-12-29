#pragma once

#include "../VulkanAPIHeader.h"

#include "BaseRenderAPI_New/Interface/Device.h"

#include "../VulkanMemoryManager.h"
#include "../VulkanCommandListPool.h"
#include "../VulkanQueueManager.h"
#include "../VulkanFencePool.h"
#include "../VulkanSemaphorePool.h"
#include "../VulkanLogicalDevice.h"
#include "../VulkanShaderParameterManager.h"
#include "../VulkanUploadHeap.h"

#include "EngineCore/Thread/MutexLock.h"

namespace cube
{
	namespace render
	{
		class DeviceVk final : public Device, public std::enable_shared_from_this<DeviceVk>
		{
		public:
			DeviceVk(VkInstance ins, SPtr<VulkanLogicalDevice>&& device);
			virtual ~DeviceVk();

			virtual SPtr<Buffer> CreateBuffer(const BufferAttribute& attr) override final;
			virtual SPtr<Texture> CreateTexture(const TextureAttribute& attr) override final;
			virtual SPtr<SwapChain> CreateSwapChain(const SwapChainAttribute& attr) override final;

			virtual SPtr<RenderTarget> CreateRenderTarget(const RenderTargetAttribute& attr) override final;
			virtual SPtr<RenderPass> CreateRenderPass(const RenderPassAttribute& attr) override final;

			virtual SPtr<ShaderParametersLayout> CreateShaderParametersLayout(const ShaderParametersLayoutAttribute& attr) override final;
			virtual SPtr<Shader> CreateShader(const ShaderAttribute& attr) override final;
			virtual SPtr<GraphicsPipelineState> CreateGraphicsPipelineState(const GraphicsPipelineStateAttribute& attr) override final;
			virtual SPtr<ComputePipelineState> CreateComputePipelineState(const ComputePipelineStateAttribute& attr) override final;

			virtual SPtr<Fence> GetFence(const char* debugName = "") override final;

			virtual SPtr<CommandList> GetCommandList(const CommandListAttribute& attr) override final;
			virtual SPtr<Fence> SubmitCommandList(SPtr<CommandList>& commandList) override final;

			virtual void StartFrame() override final;

			SPtr<VulkanLogicalDevice> GetLogicalDevice() const { return mDevice; }
			VulkanMemoryManager& GetMemoryManager() { return mMemoryManager; }
			VulkanQueueManager& GetQueueManager() { return mQueueManager; }
			VulkanShaderParameterManager& GetShaderParameterManager() { return mShaderParameterManager; }
			VulkanUploadHeap& GetUploadHeap() { return mUploadHeap; }
			
			template <typename VkObjectType>
			void ReleaseAtNextFrame(VkObjectType&& vkObject)
			{
				core::Lock lock(mReleaseQueueMutex);

				mReleaseVkObjectQueue.push_back(VkObjectStorage::Create(std::move(vkObject)));
			}

			template <typename T>
			void ReleaseAtNextFrame(SPtr<T> ptrObject)
			{
				core::Lock lock(mReleaseQueueMutex);

				mReleaseFuncQueue.push_back([ptrObject]() mutable {
					ptrObject.reset();
				});
			}

		private:
			SPtr<VulkanLogicalDevice> mDevice;
			VkInstance mInstance;

			// VulkanPhysicalDevice& mParentPhysicalDevice;
			VulkanMemoryManager mMemoryManager;

			VulkanFencePool mFencePool;
			VulkanSemaphorePool mSemaphorePool;
			VulkanQueueManager mQueueManager;
			VulkanCommandListPool mCommandListPool;
			VulkanShaderParameterManager mShaderParameterManager;
			VulkanUploadHeap mUploadHeap;

			core::Mutex mReleaseQueueMutex;
			Vector<std::function<void()>> mReleaseFuncQueue;
			Vector<VkObjectStorage> mReleaseVkObjectQueue;
		};
	} // namespace render
} // namespace cube
