#include "VulkanWindowSurface.h"

#include "Win32/Win32Platform.h"

#include "VulkanInstance.h"
#include "VulkanPhysicalDevice.h"
#include "VulkanDevice.h"
#include "VulkanQueue.h"

namespace cube
{
	namespace render
	{
#ifdef _WIN32
		VulkanWindowSurface::VulkanWindowSurface(const SPtr<VulkanInstance>& instance, const SPtr<VulkanPhysicalDevice>& physicalDevice, const SPtr<VulkanDevice>& device) :
			mInstance_ref(instance), mPhysicalDevice_ref(physicalDevice), mDevice_ref(device)
		{
			VkWin32SurfaceCreateInfoKHR info;
			info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
			info.pNext = nullptr;
			info.flags = 0;
			info.hinstance = platform::Win32Platform::GetInstance();
			info.hwnd = platform::Win32Platform::GetWindow();

			VkResult res;
			res = vkCreateWin32SurfaceKHR(instance->GetHandle(), &info, nullptr, &mSurface);
			CheckVkResult("Cannot create VulkanWindowSurface for win32", res);

			GetProperties();
		}
#endif // _WIN32

		VulkanWindowSurface::~VulkanWindowSurface()
		{
			vkDestroySurfaceKHR(mInstance_ref->GetHandle(), mSurface, nullptr);
		}

		const VkSurfaceCapabilitiesKHR VulkanWindowSurface::GetCapabilities() const
		{
			VkResult res;

			VkSurfaceCapabilitiesKHR cap;

			res = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(mPhysicalDevice_ref->GetHandle(), mSurface, &cap);
			CheckVkResult("Cannot get capabilities", res);

			return cap;
		}

		void VulkanWindowSurface::GetProperties()
		{
			VkResult res;

			// Find a present queue family
			// First, try to check if the graphics queue family supports present
			VulkanQueueFamily graphicsQueueFamily = mDevice_ref->GetGraphicsQueueFamily();
			VkBool32 b;
			vkGetPhysicalDeviceSurfaceSupportKHR(mPhysicalDevice_ref->GetHandle(), graphicsQueueFamily.mIndex, mSurface, &b);
			if(b == VK_TRUE)
				mPresentQueueFamily = graphicsQueueFamily;
			else {
				// If not, try to find present queue family
				const Vector<VulkanQueueFamily>& queueFamilies = mDevice_ref->GetAllQueueFamily();

				for(int i = 0; i < queueFamilies.size(); i++) {
					vkGetPhysicalDeviceSurfaceSupportKHR(mPhysicalDevice_ref->GetHandle(), queueFamilies[i].mIndex, mSurface, &b);

					if(b == VK_TRUE) {
						mPresentQueueFamily = queueFamilies[i];
						break;
					}
				}
			}

			// Get surface formats
			// TODO: 일단 SRGB만 지원. 차후 UNORM도 지원?? (둘의 차이가 뭔가)
			uint32_t surfaceFormatCount = 0;
			res = vkGetPhysicalDeviceSurfaceFormatsKHR(mPhysicalDevice_ref->GetHandle(), mSurface, &surfaceFormatCount, nullptr);
			CheckVkResult("Cannot find surface formats", res);

			VkSurfaceFormatKHR* surfaceFormats = new VkSurfaceFormatKHR[surfaceFormatCount];
			res = vkGetPhysicalDeviceSurfaceFormatsKHR(mPhysicalDevice_ref->GetHandle(), mSurface, &surfaceFormatCount, surfaceFormats);
			CheckVkResult("Cannot find surface formats", res);

			// If there is no supported surface format, use RGBA
			if(surfaceFormatCount == 1 && surfaceFormats[0].format == VK_FORMAT_UNDEFINED) {
				mFormat = VK_FORMAT_R8G8B8A8_SRGB;
				mColorSpace = surfaceFormats[0].colorSpace;
			} else {
				// Find supported SRGB format
				VkFormat SRGBFormats[] = {
					VK_FORMAT_B8G8R8A8_SRGB,
					VK_FORMAT_R8G8B8A8_SRGB,
					VK_FORMAT_A8B8G8R8_SRGB_PACK32,
					VK_FORMAT_B8G8R8_SRGB,
					VK_FORMAT_R8G8B8_SRGB
				};
				uint32_t SRGBFormatNum = sizeof(SRGBFormats) / sizeof(SRGBFormats[0]);

				bool isFound = false;

				for(uint32_t i = 0; i < SRGBFormatNum; i++) {
					for(uint32_t j = 0; j < surfaceFormatCount; j++) {
						if(SRGBFormats[i] == surfaceFormats[j].format) {
							mFormat = surfaceFormats[j].format;
							mColorSpace = surfaceFormats[j].colorSpace;
							isFound = true;
							break;
						}
					}
					if(isFound == true)
						break;
				}

				// If not found, use the first surface format
				if(isFound == false) {
					mFormat = surfaceFormats[0].format;
					mColorSpace = surfaceFormats[0].colorSpace;
				}
			}

			delete[] surfaceFormats;
		}
	} // namespace render
} // namespace cube
