#include "SwapChainVk.h"

#include "../VulkanUtility.h"
#include "../VulkanTypeConversion.h"
#include "../VulkanDebug.h"
#include "../VulkanPhysicalDevice.h"
#include "DeviceVk.h"
#include "TextureViewVk.h"

#include "Win32/Win32Platform.h"
#include "EngineCore/LogWriter.h"
#include "EngineCore/Assertion.h"

namespace cube
{
	namespace render
	{
		SwapChainVk::SwapChainVk(VkInstance ins, DeviceVk& device, const SwapChainAttribute& attr,
			VulkanQueueManager& queueManager, VulkanSemaphorePool& semaphorePool) :
			SwapChain(attr),
			mSwapChain(VK_NULL_HANDLE, device.GetLogicalDevice()),
			mSemaphorePool(semaphorePool),
			mQueueManager(device.GetQueueManager())
		{
			VkResult res;

			// Create os-dependent surface
#if defined(VK_USE_PLATFORM_WIN32_KHR)
			VkWin32SurfaceCreateInfoKHR info;
			info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
			info.pNext = nullptr;
			info.flags = 0;
			info.hinstance = platform::Win32Platform::GetInstance();
			info.hwnd = platform::Win32Platform::GetWindow();

			res = vkCreateWin32SurfaceKHR(ins, &info, nullptr, &mSurface);
#endif
			CHECK_VK(res, "Failed to create surface.");

			auto& physicalDevice = mSwapChain.GetVulkanLogicalDevice()->GetParentPhysicalDevice();
			vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice.GetHandle(), mSurface, &mSurfaceCapabilities);

			// Find color / depth surface
			// TODO: depth도?
			Uint32 formatCount = 0;
			res = vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice.GetHandle(), mSurface, &formatCount, nullptr);
			CHECK_VK(res, "Failed to get number of supported formats.");
			Vector<VkSurfaceFormatKHR> supportedFormats(formatCount);
			res = vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice.GetHandle(), mSurface, &formatCount, supportedFormats.data());
			CHECK_VK(res, "Failed to get supported formats.");

			mColorFormat = TextureFormatToVkFormat(attr.colorBufferFormat);
			mDepthFormat = TextureFormatToVkFormat(attr.depthBufferFormat);

			bool isFound = false;
			for(auto& fmt : supportedFormats) {
				if(fmt.format == mColorFormat) {
					isFound = true;
					mColorSpace = fmt.colorSpace;
					break;
				}
			}
			if(!isFound) {
				// Use the solution of DiligentEngine
				// https://github.com/DiligentGraphics/DiligentEngine (SwapChainVkImpl.cpp:139)
				// TODO: UE4의 솔루션도 참고
				VkFormat alterFormat = VK_FORMAT_UNDEFINED;
				switch(mColorFormat) {
					case VK_FORMAT_R8G8B8A8_UNORM: alterFormat = VK_FORMAT_B8G8R8A8_UNORM; break;
					case VK_FORMAT_B8G8R8A8_UNORM: alterFormat = VK_FORMAT_R8G8B8A8_UNORM; break;
					case VK_FORMAT_B8G8R8A8_SRGB: alterFormat = VK_FORMAT_R8G8B8A8_SRGB; break;
					case VK_FORMAT_R8G8B8A8_SRGB: alterFormat = VK_FORMAT_B8G8R8A8_SRGB; break;
				}

				isFound = false;
				for(auto& fmt : supportedFormats) {
					if(fmt.format == alterFormat) {
						isFound = true;
						mColorSpace = fmt.colorSpace;
						break;
					}
				}

				if(isFound) {
					CUBE_LOG(LogType::Info, "The color image format '{0}' is not supported. So it will be replaced with '{1}'.",
						mColorFormat, alterFormat);
					mColorFormat = alterFormat;
				} else {
					CUBE_LOG(LogType::Info, "The color image format '{0}' is not supported and it cannot be replaced similar. So it will be used the first supported format '{1}'.",
						mColorFormat, supportedFormats[0].format);
					mColorFormat = supportedFormats[0].format;
					mColorSpace = supportedFormats[0].colorSpace;
				}
			}

			// Find present mode for normal and vsync
			Uint32 presentCount = 0;
			res = vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice.GetHandle(), mSurface, &presentCount, nullptr);
			CHECK_VK(res, "Failed to get number of supported present mode.");
			Vector<VkPresentModeKHR> supportedPresentModes(presentCount);
			res = vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice.GetHandle(), mSurface,
				&presentCount, supportedPresentModes.data());
			CHECK_VK(res, "Failed to get supported present mode.");

			mPresentMode = VK_PRESENT_MODE_MAILBOX_KHR;
			mVsyncPresentMode = VK_PRESENT_MODE_FIFO_KHR;
			mIsSupportedVsync = true;

			isFound = std::find(supportedPresentModes.cbegin(), supportedPresentModes.cend(), mPresentMode) != supportedPresentModes.cend();
			if(!isFound) {
				mPresentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
				isFound = std::find(supportedPresentModes.cbegin(), supportedPresentModes.cend(), mPresentMode) != supportedPresentModes.cend();
				
				if(!isFound) {
					ASSERTION_FAILED("Cannot find supported present mode (MAILBOX or IMMEDIATE).");
				}
			}
			
			isFound = std::find(supportedPresentModes.cbegin(), supportedPresentModes.cend(), mVsyncPresentMode) != supportedPresentModes.cend();
			if(!isFound) {
				mIsSupportedVsync = false;
			}

			// Get a present queue
			mPresentQueue = queueManager.GetPresentQueue(mSurface);

			CreateSwapChain();
			CreateImages();
		}

		SwapChainVk::~SwapChainVk()
		{
			for(auto view : mBackImageViews) {
				vkDestroyImageView(mSwapChain.GetVkDevice(), view, nullptr);
			}

			mSwapChain.Release();
		}

		void SwapChainVk::AcquireNextImage()
		{
			VkResult res;

			VulkanSemaphore signalSemaphore = mSemaphorePool.GetSemaphore();

			res = vkAcquireNextImageKHR(mSwapChain.GetVkDevice(), mSwapChain.mObject, UINT64_MAX,
				signalSemaphore.handle, VK_NULL_HANDLE, &mCurrentBackImageIndex);
			CHECK_VK(res, "Failed to acquire next image.");

			mQueueManager.AddWaitSemaphoreForGraphics(signalSemaphore, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
		}

		void SwapChainVk::Present()
		{
			VkResult res;

			VkPresentInfoKHR info;
			info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
			info.pNext = nullptr;
			info.swapchainCount = 1;
			info.pSwapchains = &mSwapChain.mObject;
			info.pImageIndices = &mCurrentBackImageIndex;
			// TODO: 일단 Fence로 draw가 끝날 때 Present를 하니까 waitSemaphore는 없어도 될 듯
			// info.waitSemaphoreCount = 1;
			// info.pWaitSemaphores = &mDrawCompleteSemaphores[mCurrentBackImageIndex].handle;
			info.waitSemaphoreCount = 0;
			info.pWaitSemaphores = nullptr;
			info.pResults = nullptr;

			res = vkQueuePresentKHR(mPresentQueue, &info);
			CHECK_VK(res, "Failed to present.");
		}

		void SwapChainVk::Resize(Uint32 width, Uint32 height, bool vsync)
		{
			if(vsync == true) {
				CHECK(mIsSupportedVsync == true, "Vsync is not supported this platform.");
			}

			// Release all resources related to swap chain
			for(auto imgView : mBackImageViews) {
				vkDestroyImageView(mSwapChain.GetVkDevice(), imgView, nullptr);
			}
			mBackImageViews.clear();
			for(auto& semaphore : mDrawCompleteSemaphores) {
				mSemaphorePool.FreeSemaphore(semaphore);
			}
			mDrawCompleteSemaphores.clear();

			mWidth = width;
			mHeight = height;
			mVsync = vsync;

			CreateSwapChain();
			CreateImages();
		}

		void SwapChainVk::SetFullscreenMode()
		{
			// TODO: 차후 구현
		}

		void SwapChainVk::SetWindowedMode()
		{
			// TODO: 차후 구현
		}

		void SwapChainVk::CreateSwapChain()
		{
			VkSwapChainWrapper oldSwapChain = std::move(mSwapChain);
			VulkanDebug::SetObjectName(oldSwapChain.GetVkDevice(), oldSwapChain.mObject, "Old SwapChain");

			// Clamp width / height
			if(mSurfaceCapabilities.maxImageExtent.width < mWidth)
				mWidth = mSurfaceCapabilities.maxImageExtent.width;
			if(mSurfaceCapabilities.maxImageExtent.height < mHeight)
				mHeight = mSurfaceCapabilities.maxImageExtent.height;
			if(mSurfaceCapabilities.minImageExtent.width > mWidth)
				mWidth = mSurfaceCapabilities.minImageExtent.width;
			if(mSurfaceCapabilities.minImageExtent.height > mHeight)
				mHeight = mSurfaceCapabilities.minImageExtent.height;

			// Clamp image(buffer) count
			if(mSurfaceCapabilities.maxImageCount < mBufferCount)
				mBufferCount = mSurfaceCapabilities.maxImageCount;
			if(mSurfaceCapabilities.minImageCount > mBufferCount)
				mBufferCount = mSurfaceCapabilities.minImageCount;

			VkSwapchainCreateInfoKHR info = {};
			info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
			info.pNext = nullptr;
			info.flags = 0;
			info.surface = mSurface;
			info.minImageCount = mBufferCount;
			info.imageFormat = mColorFormat;
			info.imageColorSpace = mColorSpace;
			info.imageExtent = { mWidth, mHeight };
			info.imageArrayLayers = 1;
			info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
			info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			info.queueFamilyIndexCount = 0;
			info.pQueueFamilyIndices = nullptr;
			info.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
			info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
			if(mVsync == true)
				info.presentMode = mVsyncPresentMode;
			else
				info.presentMode = mPresentMode;
			info.clipped = VK_TRUE;
			info.oldSwapchain = oldSwapChain.mObject;

			mSwapChain = oldSwapChain.GetVulkanLogicalDevice()->CreateVkSwapChainWrapper(info, "SwapChain");
			oldSwapChain.Release();

			// Create semaphores
			/*mAcquiredSemaphores.resize(mImageCount);
			for(Uint32 i = 0; i < mImageCount; i++) {
				mAcquiredSemaphores.push_back(mSemaphorePool.GetSemaphore());
			}*/
			mDrawCompleteSemaphores.resize(mBufferCount);
			for(Uint32 i = 0; i < mBufferCount; i++) {
				mDrawCompleteSemaphores[i] = mSemaphorePool.GetSemaphore(fmt::format("DrawCompleteSemaphores[{0}]", i).c_str());
			}
		}

		void SwapChainVk::CreateImages()
		{
			VkResult res;

			Uint32 imageCount = 0;
			res = vkGetSwapchainImagesKHR(mSwapChain.GetVkDevice(), mSwapChain.mObject, &imageCount, nullptr);
			CHECK_VK(res, "Failed to get number of swap chain images.");
			Vector<VkImage> vkImages(imageCount);
			res = vkGetSwapchainImagesKHR(mSwapChain.GetVkDevice(), mSwapChain.mObject, &imageCount, vkImages.data());
			CHECK_VK(res, "Failed to get swap chain images.");

			mBackImageViews.resize(imageCount);

			for(Uint32 i = 0; i < imageCount; i++) {
				VkImageViewCreateInfo info = {};
				info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
				info.pNext = nullptr;
				info.flags = 0;
				info.image = vkImages[i];
				info.viewType = VK_IMAGE_VIEW_TYPE_2D;
				info.format = mColorFormat;
				info.components = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
				info.subresourceRange.baseMipLevel = 0;
				info.subresourceRange.levelCount = 1;
				info.subresourceRange.baseArrayLayer = 0;
				info.subresourceRange.layerCount = 1;
				info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

				res = vkCreateImageView(mSwapChain.GetVkDevice(), &info, nullptr, &mBackImageViews[i]);
				CHECK_VK(res, "Failed to create swapchain back image view.");

				U8String debugName = fmt::format("SwapChain BackImageView({0})", i);
				VulkanDebug::SetObjectName(mSwapChain.GetVkDevice(), mBackImageViews[i], debugName.c_str());
			}
			
			// TODO: DepthImage도?
		}
	} // namespace render
} // namespace cube