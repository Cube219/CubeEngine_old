#include "DeviceVk.h"

#include "../VulkanUtility.h"
#include "../VulkanDebug.h"
#include "../VulkanPhysicalDevice.h"
#include "BaseRenderAPI_New/Interface/Buffer.h"
#include "BufferVk.h"
#include "FenceVk.h"
#include "CommandListVk.h"

namespace cube
{
	namespace render
	{
		DeviceVk::DeviceVk(SPtr<VulkanLogicalDevice> device) : 
			mDevice(device),
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

		SPtr<CommandList> DeviceVk::GetCommandList(CommandListUsage usage)
		{
			return mCommandListPool.Allocate(usage);
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
