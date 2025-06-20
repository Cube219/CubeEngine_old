#include "DeviceVk.h"

#include "../VulkanUtility.h"
#include "../VulkanDebug.h"
#include "RenderAPI/Interface/Buffer.h"
#include "BufferVk.h"
#include "FenceVk.h"
#include "CommandListVk.h"
#include "TextureVk.h"
#include "SwapChainVk.h"
#include "RenderTargetVk.h"
#include "RenderPassVk.h"
#include "ShaderParametersLayoutVk.h"
#include "ShaderVk.h"
#include "GraphicsPipelineStateVk.h"
#include "ComputePipelineStateVk.h"
#include "FenceVk.h"
#include "SamplerVk.h"

namespace cube
{
	namespace render
	{
		DeviceVk::DeviceVk(VkInstance ins, SPtr<VulkanLogicalDevice>&& device, const DeviceAttribute& attr) :
			Device(attr),
			mInstance(ins),
			mDevice(std::move(device)),
			mMemoryManager(mDevice, 256*1024*1024, 32*1024*1024), // gpuPage: 256 MiB, hostVisiblePage: 32 MiB
			mFencePool(mDevice),
			mSemaphorePool(mDevice),
			mQueueManager(mDevice, mDevice->GetParentPhysicalDevice(), mFencePool, mSemaphorePool),
			mCommandListPool(mDevice, mQueueManager),
			mShaderParameterManager(mDevice, mMemoryManager),
			mUploadHeap(mMemoryManager, 32*1024*1024) // Page: 32 MiB
		{
		}

		DeviceVk::~DeviceVk()
		{
		}

		SPtr<Buffer> DeviceVk::CreateBuffer(const BufferAttribute& attr)
		{
			return std::make_shared<BufferVk>(*this, attr, mQueueManager, mCommandListPool);
		}

		SPtr<CommandList> DeviceVk::GetCommandList(const CommandListAttribute& attr)
		{
			return mCommandListPool.Allocate(attr);
		}

		SPtr<Texture> DeviceVk::CreateTexture(const TextureAttribute& attr)
		{
			return std::make_shared<TextureVk>(*this, attr, mQueueManager, mCommandListPool);
		}

		SPtr<SwapChain> DeviceVk::CreateSwapChain(const SwapChainAttribute& attr)
		{
			return std::make_shared<SwapChainVk>(mInstance, *this, attr, mQueueManager, mSemaphorePool);
		}

		SPtr<Sampler> DeviceVk::CreateSampler(const SamplerAttribute& attr)
		{
			return std::make_shared<SamplerVk>(*this, attr);
		}

		SPtr<RenderTarget> DeviceVk::CreateRenderTarget(const RenderTargetAttribute& attr)
		{
			return std::make_shared<RenderTargetVk>(attr);
		}

		SPtr<RenderPass> DeviceVk::CreateRenderPass(const RenderPassAttribute& attr)
		{
			return std::make_shared<RenderPassVk>(*this, attr);
		}

		SPtr<ShaderParametersLayout> DeviceVk::CreateShaderParametersLayout(const ShaderParametersLayoutAttribute& attr)
		{
			return std::make_shared<ShaderParametersLayoutVk>(*this, attr);
		}

		SPtr<Shader> DeviceVk::CreateShader(const ShaderAttribute& attr)
		{
			return std::make_shared<ShaderVk>(*this, attr);
		}

		SPtr<GraphicsPipelineState> DeviceVk::CreateGraphicsPipelineState(const GraphicsPipelineStateAttribute& attr)
		{
			return std::make_shared<GraphicsPipelineStateVk>(*this, attr);
		}

		SPtr<ComputePipelineState> DeviceVk::CreateComputePipelineState(const ComputePipelineStateAttribute& attr)
		{
			return std::make_shared<ComputePipelineStateVk>(*this, attr);
		}

		SPtr<Fence> DeviceVk::GetFence(const char* debugName)
		{
			return mFencePool.GetFence(debugName);;
		}

		void DeviceVk::SubmitCommandList(SPtr<CommandList>& commandList)
		{
			SPtr<CommandListVk> commandListVk = DPCast(CommandListVk)(commandList);
			mQueueManager.SubmitCommandList(*commandListVk);
		}

		SPtr<Fence> DeviceVk::SubmitCommandListWithFence(SPtr<CommandList>& commandList)
		{
			SPtr<CommandListVk> commandListVk = DPCast(CommandListVk)(commandList);
			return mQueueManager.SubmitCommandListWithFence(*commandListVk);
		}

		void DeviceVk::StartFrame()
		{
			Lock lock(mReleaseQueueMutex);

			for(auto& func : mReleaseFuncQueue) {
				func();
			}
			mReleaseFuncQueue.clear();
			mReleaseVkObjectQueue.clear();

			mShaderParameterManager.DiscardPerFrame();
			mUploadHeap.DiscardAllocations();
		}
	} // namespace render
} // namespace cube
