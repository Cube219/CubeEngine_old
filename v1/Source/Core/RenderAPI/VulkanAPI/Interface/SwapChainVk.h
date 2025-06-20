#pragma once

#include "../VulkanAPIHeader.h"

#include "RenderAPI/Interface/SwapChain.h"

#include "../VkObject.h"
#include "../VulkanSemaphorePool.h"

namespace cube
{
	namespace render
	{
		class SwapChainVk final : public SwapChain
		{
		public:
			SwapChainVk(VkInstance ins, DeviceVk& device, const SwapChainAttribute& attr,
				VulkanQueueManager& queueManager, VulkanSemaphorePool& semaphorePool);
			virtual ~SwapChainVk();

			virtual void AcquireNextImage() override final;
			virtual void Present() override final;

			virtual void Resize(Uint32 width, Uint32 height, bool vsync) override final;

			virtual void SetFullscreenMode() override final;
			virtual void SetWindowedMode() override final;

			Uint32 GetCurrentBackImageIndex() const { return mCurrentBackImageIndex; }
			VulkanSemaphore& GetDrawCompleteSemaphore() { return mDrawCompleteSemaphores[mCurrentBackImageIndex]; }
			const Vector<VkImageView>& GetBackImages() const { return mBackImageViews; }

			VkFormat GetColorVkFormat() const { return mColorFormat; }
			Uint32 GetImageCount() const { return mBufferCount; }

		private:
			void CreateSwapChain();
			void CreateImages();

			VulkanSemaphorePool& mSemaphorePool;
			VulkanQueueManager& mQueueManager;

			VkSwapChainWrapper mSwapChain;

			bool mIsSupportedVsync;

			VkSurfaceKHR mSurface;
			VkSurfaceCapabilitiesKHR mSurfaceCapabilities;
			VkFormat mColorFormat;
			VkFormat mDepthFormat;
			VkColorSpaceKHR mColorSpace;
			VkPresentModeKHR mPresentMode;
			VkPresentModeKHR mVsyncPresentMode;

			Vector<VkImageView> mBackImageViews;
			// TODO: 세마포어를 이용해 병렬처리?
			//       지금은 얻는데 block됨
			// Vector<VulkanSemaphore> mAcquiredSemaphores;
			Vector<VulkanSemaphore> mDrawCompleteSemaphores;
			Uint32 mCurrentBackImageIndex;

			VkQueue mPresentQueue;
		};
	} // namespace render
} // namespace cube
