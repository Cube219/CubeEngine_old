#include "VulkanSwapchain.h"

#include "VulkanDevice.h"
#include "VulkanWindowSurface.h"
#include "VulkanImage.h"
#include "VulkanSemaphore.h"
#include "VulkanQueue.h"

namespace cube
{
	namespace render
	{
		VulkanSwapchain::VulkanSwapchain(const SPtr<VulkanDevice>& device, const SPtr<VulkanWindowSurface>& surface,
			uint32_t imageCount, uint32_t width, uint32_t height, bool vsync) :
			mDevice_ref(device), mSurface_ref(surface), mSwapchain(nullptr)
		{
			mPresentQueue = device->GetQueue(surface->GetPresentQueueFamily(), 0);

			CreateSwapchain(false, imageCount, width, height, vsync);
		}

		VulkanSwapchain::~VulkanSwapchain()
		{
			mImages.clear();
			mImageViews.clear();

			vkDestroySwapchainKHR(mDevice_ref->GetHandle(), mSwapchain, nullptr);
		}

		uint32_t VulkanSwapchain::AcquireNextImageIndex(SPtr<Semaphore>& signalSemaphore)
		{
			VkResult res;

			res = vkAcquireNextImageKHR(mDevice_ref->GetHandle(), mSwapchain, UINT64_MAX, DPCast(VulkanSemaphore)(signalSemaphore)->GetHandle(), VK_NULL_HANDLE, &mCurrentImageIndex);
			CheckVkResult("Cannot get a next iamge", res);

			return mCurrentImageIndex;
		}

		void VulkanSwapchain::Recreate(uint32_t imageCount, uint32_t width, uint32_t height, bool vsync)
		{
			CreateSwapchain(true, imageCount, width, height, vsync);
		}

		void VulkanSwapchain::Present(uint32_t waitSemaphoreNum, SPtr<Semaphore>* waitSemaphores)
		{
			VkResult res;

			std::vector<VkSemaphore> wait;
			wait.resize(waitSemaphoreNum);
			for(uint32_t i = 0; i < waitSemaphoreNum; i++) {
				wait[i] = DPCast(VulkanSemaphore)(waitSemaphores[i])->GetHandle();
			}

			VkPresentInfoKHR info;
			info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
			info.pNext = nullptr;
			info.swapchainCount = 1;
			info.pSwapchains = &mSwapchain;
			info.pImageIndices = &mCurrentImageIndex;
			info.waitSemaphoreCount = waitSemaphoreNum;
			info.pWaitSemaphores = wait.data();
			info.pResults = nullptr;

			res = vkQueuePresentKHR(mPresentQueue->GetHandle(), &info);
			CheckVkResult("Cannot present", res);

			res = vkQueueWaitIdle(mPresentQueue->GetHandle());
			CheckVkResult("Cannot wait the present queue", res);
		}

		// TODO: vsync 기능 구현
		void VulkanSwapchain::CreateSwapchain(bool isRecreated, uint32_t imageCount, uint32_t width, uint32_t height, bool vsync)
		{
			VkResult res;

			// Clear the images
			mImages.clear();

			VkSwapchainKHR oldSwapchain = VK_NULL_HANDLE;
			if(isRecreated == true)
				oldSwapchain = mSwapchain;

			// Get surface capabilities
			const VkSurfaceCapabilitiesKHR surfaceCapabilities = mSurface_ref->GetCapabilities();

			// Clamp image count
			if(surfaceCapabilities.minImageCount > imageCount)
				imageCount = surfaceCapabilities.minImageCount;
			if(surfaceCapabilities.maxImageCount < imageCount)
				imageCount = surfaceCapabilities.maxImageCount;

			// Create swapchain
			VkSwapchainCreateInfoKHR info = {};
			info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
			info.pNext = nullptr;
			info.flags = 0;
			info.surface = mSurface_ref->GetHandle();
			info.minImageCount = imageCount;
			info.imageFormat = mSurface_ref->GetFormat();
			info.imageColorSpace = mSurface_ref->GetColorSpace();
			info.imageExtent = {width, height};
			info.imageArrayLayers = 1;
			info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
			info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			info.queueFamilyIndexCount = 0;
			info.pQueueFamilyIndices = nullptr;
			info.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
			info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
			info.presentMode = VK_PRESENT_MODE_MAILBOX_KHR;
			info.clipped = VK_TRUE;
			info.oldSwapchain = oldSwapchain;

			// If it is different present queue and graphics queue, change info slightly
			uint32_t queueFamilyIndices[2] = {
				mDevice_ref->GetGraphicsQueueFamily().mIndex,
				mSurface_ref->GetPresentQueueFamily().mIndex
			};
			if(queueFamilyIndices[0] != queueFamilyIndices[1]) {
				info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
				info.queueFamilyIndexCount = 2;
				info.pQueueFamilyIndices = queueFamilyIndices;
			}

			res = vkCreateSwapchainKHR(mDevice_ref->GetHandle(), &info, nullptr, &mSwapchain);
			CheckVkResult("Cannot create VulkanSwapchain", res);

			// If this swapchain is recreated, destroy the old swapchain
			if(isRecreated == true) {
				mImages.clear();

				vkDestroySwapchainKHR(mDevice_ref->GetHandle(), oldSwapchain, nullptr);
			}

			// Get the swapchain imgaes
			uint32_t swapchainImageCount = 0;
			res = vkGetSwapchainImagesKHR(mDevice_ref->GetHandle(), mSwapchain, &swapchainImageCount, nullptr);
			CheckVkResult("Cannt get image number in the swapchain", res);

			VkImage* imgs = new VkImage[swapchainImageCount];
			res = vkGetSwapchainImagesKHR(mDevice_ref->GetHandle(), mSwapchain, &swapchainImageCount, imgs);
			CheckVkResult("Cannt get images in the swapchain", res);

			mImages.resize(swapchainImageCount);
			mImageViews.resize(swapchainImageCount);
			for(uint32_t i = 0; i < swapchainImageCount; i++) {
				mImages[i] = std::make_shared<VulkanImage>(mDevice_ref, imgs[i]);

				// Create a image view of each color images in the swapchain
				VkComponentMapping mapping = {VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A};
				VkImageSubresourceRange subresource = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1};
				mImageViews[i] = std::make_shared<VulkanImageView>(mDevice_ref, mImages[i],
					mSurface_ref->GetFormat(), mapping, subresource, VK_IMAGE_VIEW_TYPE_2D);
			}

			delete[] imgs;

			mWidth = width;
			mHeight = height;
		}
	} // namespace render
} // namespace cube
