#pragma once

#include "..\VulkanAPIHeader.h"

#include "BaseRenderAPI\Wrapper\BaseRenderSwapchain.h"

namespace cube
{
	namespace core
	{
		struct VulkanSwapchainImage; // Defined at VulkanAPIHeader.h

		class VULKAN_API_EXPORT VulkanSwapchain : public BaseRenderSwapchain
		{
		public:
			VulkanSwapchain(const SPtr<VulkanDevice>& device, const SPtr<VulkanWindowSurface>& surface,
				uint32_t imageCount, uint32_t width, uint32_t height, bool vsync);
			virtual ~VulkanSwapchain();

			const uint32_t GetImageCount() const;
			const Vector<SPtr<VulkanImageView>>& GetImageViews() const;
			uint32_t GetWidth() const;
			uint32_t GetHeight() const;

			uint32_t GetCurrentImageIndex() const;

			uint32_t AcquireNextImageIndex(SPtr<BaseRenderSemaphore>& signalSemaphore) override;

			void Recreate(uint32_t imageCount, uint32_t width, uint32_t height, bool vsync) override;

			void Present(uint32_t waitSemaphoreNum, SPtr<BaseRenderSemaphore>* waitSemaphores) override;
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

		inline const uint32_t VulkanSwapchain::GetImageCount() const
		{
			return SCast(uint32_t)(mImages.size());
		}

		inline const Vector<SPtr<VulkanImageView>>& VulkanSwapchain::GetImageViews() const
		{
			return mImageViews;
		}

		inline uint32_t VulkanSwapchain::GetWidth() const
		{
			return mWidth;
		}

		inline uint32_t VulkanSwapchain::GetHeight() const
		{
			return mHeight;
		}

		inline uint32_t VulkanSwapchain::GetCurrentImageIndex() const
		{
			return mCurrentImageIndex;
		}
	}
}