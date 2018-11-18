#include "SwapChainVk.h"

#include "../VulkanUtility.h"
#include "../VulkanTypeConversion.h"
#include "../VulkanDebug.h"
#include "../VulkanPhysicalDevice.h"
#include "DeviceVk.h"

#include "Win32/Win32Platform.h"
#include "EngineCore/LogWriter.h"
#include "EngineCore/Assertion.h"

namespace cube
{
	namespace render
	{
		SwapChainVk::SwapChainVk(VkInstance ins, DeviceVk& device, const SwapChainAttribute& attr,
			VulkanQueueManager& queueManager) :
			mSwapChain(VK_NULL_HANDLE, device.GetLogicalDevice()),
			mAttribute(attr),
			mWidth(attr.width), mHeight(attr.height), mImageCount(attr.bufferCount), mVsync(attr.vsync)
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

			// Find color / depth surface
			// TODO: depth도?
			auto& physicalDevice = mSwapChain.GetVulkanLogicalDevice()->GetParentPhysicalDevice();
			VkSwapchainKHR oldSwapChain = mSwapChain.mObject;

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
					CUBE_LOG(LogType::Warning, "The color image format '{0}' is not supported. So it will be replaced with '{1}'.",
						mColorFormat, alterFormat);
					mColorFormat = alterFormat;
				} else {
					CUBE_LOG(LogType::Warning, "The color image format '{0}' is not supported and it cannot be replaced similar. So it will be used the first supported format '{1}'.",
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
					ASSERTION_FAILED("Cannot find supported present mode (MAILBOX / IMMEDIATE).");
				}
			}
			
			isFound = std::find(supportedPresentModes.cbegin(), supportedPresentModes.cend(), mVsyncPresentMode) != supportedPresentModes.cend();
			if(!isFound) {
				mIsSupportedVsync = false;
			}

			// Get a present queue
			mPresentQueue = queueManager.GetPresentQueue(mSurface);

			CreateSwapChain();
		}

		SwapChainVk::~SwapChainVk()
		{
			mSwapChain.Release();
		}

		void SwapChainVk::Present()
		{
		}

		void SwapChainVk::Resize(Uint32 width, Uint32 height, bool vsync)
		{
			if(vsync == true) {
				CHECK(mIsSupportedVsync == true, "Vsync is not supported this platform.");
			}
		}

		void SwapChainVk::SetFullscreenMode()
		{
		}

		void SwapChainVk::SetWindowedMode()
		{
		}

		void SwapChainVk::CreateSwapChain()
		{
			VkSwapChainWrapper oldSwapChain = std::move(mSwapChain);
			VulkanDebug::SetObjectName(oldSwapChain.GetVkDevice(), oldSwapChain.mObject, "Old SwapChain");

			VkSwapchainCreateInfoKHR info = {};
			info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
			info.pNext = nullptr;
			info.flags = 0;
			info.surface = mSurface;
			info.minImageCount = mImageCount;
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
		}

		void SwapChainVk::CreateImages()
		{
			VkResult res;

			Uint32 imageCount = 0;
			res = vkGetSwapchainImagesKHR(mSwapChain.GetVkDevice(), mSwapChain.mObject, &imageCount, nullptr);
			CHECK_VK(res, "Failed to get number of swap chain images.");
			Vector<VkImage> vkImages;
			res = vkGetSwapchainImagesKHR(mSwapChain.GetVkDevice(), mSwapChain.mObject, &imageCount, vkImages.data());
			CHECK_VK(res, "Failed to get swap chain images.");

			for(Uint32 i = 0; i < imageCount; i++) {
				// TODO: TextureView를 만들면 하기
			}

		}
	} // namespace render
} // namespace cube