#pragma once

#include "../VulkanAPIHeader.h"

#include "BaseRenderAPI/Wrapper/Swapchain.h"

namespace cube
{
	namespace render
	{
		struct VulkanSwapchainImage; // Defined at VulkanAPIHeader.h

		class VULKAN_API_EXPORT VulkanSwapchain : public Swapchain
		{
		public:
			VulkanSwapchain(const SPtr<VulkanDevice>& device, const SPtr<VulkanWindowSurface>& surface,
				uint32_t imageCount, uint32_t width, uint32_t height, bool vsync);
			virtual ~VulkanSwapchain();

			const uint32_t GetImageCount() const { return SCast(uint32_t)(mImages.size()); }
			const Vector<SPtr<VulkanImageView>>& GetImageViews() const { return mImageViews; }
			uint32_t GetWidth() const { return mWidth; }
			uint32_t GetHeight() const { return mHeight; }

			uint32_t GetCurrentImageIndex() const { return mCurrentImageIndex; }

			uint32_t AcquireNextImageIndex(SPtr<Semaphore>& signalSemaphore) final override;

			void Recreate(uint32_t imageCount, uint32_t width, uint32_t height, bool vsync) final override;

			void Present(uint32_t waitSemaphoreNum, SPtr<Semaphore>* waitSemaphores) final override;
		private:
			void CreateSwapchain(bool isRecreated, uint32_t imageCount, uint32_t width, uint32_t height, bool vsync);

			VkSwapchainKHR mSwapchain;

			Vector<SPtr<VulkanImage>> mImages;
			Vector<SPtr<VulkanImageView>> mImageViews;
			uint32_t mCurrentImageIndex;

			uint32_t mWidth;
			uint32_t mHeight;

			SPtr<VulkanQueue> mPresentQueue;

			SPtr<VulkanDevice> mDevice_ref;
			SPtr<VulkanWindowSurface> mSurface_ref;
		};
	} // namespace render
} // namespace cube
