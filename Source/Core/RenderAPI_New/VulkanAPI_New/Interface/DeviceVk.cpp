#include "DeviceVk.h"

#include "../VulkanUtility.h"
#include "../VulkanDebug.h"
#include "../VulkanPhysicalDevice.h"
#include "BaseRenderAPI_New/Interface/Buffer.h"
#include "BufferVk.h"
#include "FenceVk.h"
#include "CommandListVk.h"
#include "TextureVk.h"
#include "SwapChainVk.h"

namespace cube
{
	namespace render
	{
		DeviceVk::DeviceVk(VkInstance ins, SPtr<VulkanLogicalDevice>&& device) :
			mInstance(ins),
			mDevice(std::move(device)),
			mMemoryManager(mDevice, 256*1024*1024, 32*1024*1024), // gpuPage: 256 MiB, hostVisiblePage: 32 Mib
			mFencePool(mDevice),
			mSemaphorePool(mDevice),
			mQueueManager(mDevice, mDevice->GetParentPhysicalDevice(), mFencePool, mSemaphorePool),
			mCommandListPool(mDevice, mQueueManager)
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
			return mCommandListPool.Allocate(attr.usage, attr.threadIndex, attr.isSub);
		}

		SPtr<Texture> DeviceVk::CreateTexture(const TextureAttribute& attr)
		{
			return std::make_shared<TextureVk>(*this, attr, mQueueManager, mCommandListPool);
		}

		SPtr<SwapChain> DeviceVk::CreateSwapChain(const SwapChainAttribute& attr)
		{
			return std::make_shared<SwapChainVk>(mInstance, *this, attr, mQueueManager, mSemaphorePool);
		}

		SPtr<Fence> DeviceVk::SubmitCommandList(SPtr<CommandList>& commandList)
		{
			SPtr<CommandListVk> commandListVk = DPCast(CommandListVk)(commandList);
			return mQueueManager.SubmitCommandList(*commandListVk);
		}

		void DeviceVk::FinishFrame()
		{
			core::Lock lock(mReleaseQueueMutex);

			mReleaseFuncQueue.clear();

			for(auto& func : mReleaseFuncQueue) {
				func();
			}
			mReleaseFuncQueue.clear();
		}
	} // namespace render
} // namespace cube
