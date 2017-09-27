#pragma once

#include "..\VulkanAPIHeader.h"

#include "BaseRenderAPI\Wrapper\BaseRenderWindowSurface.h"

namespace cube
{
	namespace core
	{
		class VULKAN_API_EXPORT VulkanWindowSurface : public BaseRenderWindowSurface
		{
		public:
#ifdef _WIN32
			VulkanWindowSurface(const SPtr<VulkanInstance>& instance, const SPtr<VulkanPhysicalDevice>& physicalDevice,
				HINSTANCE win32Instance, HWND win32Window);
#endif // WIN32
			virtual ~VulkanWindowSurface();

			operator VkSurfaceKHR() const
			{
				return mSurface;
			}

			VkFormat GetFormat() const;
			VkColorSpaceKHR GetColorSpace() const;
			const VkSurfaceCapabilitiesKHR GetCapabilities() const;
			VulkanQueueFamily GetPresentQueueFamily() const;

		private:
			void GetProperties();

			VkSurfaceKHR mSurface;

			VkFormat mFormat;
			VkColorSpaceKHR mColorSpace;
			VulkanQueueFamily mPresentQueueFamily;

			SPtr<VulkanInstance> mInstance_ref;
			SPtr<VulkanPhysicalDevice> mPhysicalDevice_ref;
		};

		inline VkFormat VulkanWindowSurface::GetFormat() const
		{
			return mFormat;
		}

		inline VkColorSpaceKHR VulkanWindowSurface::GetColorSpace() const
		{
			return mColorSpace;
		}

		inline VulkanQueueFamily VulkanWindowSurface::GetPresentQueueFamily() const
		{
			return mPresentQueueFamily;
		}
	}
}