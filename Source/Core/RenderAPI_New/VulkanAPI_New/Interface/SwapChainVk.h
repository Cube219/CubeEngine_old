#pragma once

#include "../VulkanAPIHeader.h"

#include "BaseRenderAPI_New/Interface/SwapChain.h"

#include "../VkObject.h"

namespace cube
{
	namespace render
	{
		class SwapChainVk final : public SwapChain
		{
		public:
			SwapChainVk(VkInstance ins, DeviceVk& device, const SwapChainAttribute& attr,
				VulkanQueueManager& queueManager);
			virtual ~SwapChainVk();

			virtual void Present() override final;
			virtual void Resize(Uint32 width, Uint32 height, bool vsync) override final;

			virtual void SetFullscreenMode() override final;
			virtual void SetWindowedMode() override final;

		private:
			void CreateSwapChain();
			void CreateImages();

			VkSwapChainWrapper mSwapChain;
			SwapChainAttribute mAttribute;

			Uint32 mWidth;
			Uint32 mHeight;
			Uint32 mImageCount;
			bool mVsync;
			bool mIsSupportedVsync;

			VkSurfaceKHR mSurface;
			VkSurfaceCapabilitiesKHR mSurfaceCapabilities;
			VkFormat mColorFormat;
			VkFormat mDepthFormat;
			VkColorSpaceKHR mColorSpace;
			VkPresentModeKHR mPresentMode;
			VkPresentModeKHR mVsyncPresentMode;

			Vector<VkImageView> mBackImageViews;

			VkQueue mPresentQueue;
		};
	} // namespace render
} // namespace cube
