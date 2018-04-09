#pragma once

#include "../VulkanAPIHeader.h"

#include "BaseRenderAPI/Wrapper/WindowSurface.h"

namespace cube
{
	namespace render
	{
		class VULKAN_API_EXPORT VulkanWindowSurface : public WindowSurface
		{
		public:
#ifdef _WIN32
			VulkanWindowSurface(const SPtr<VulkanInstance>& instance, const SPtr<VulkanPhysicalDevice>& physicalDevice, const SPtr<VulkanDevice>& device);
#endif // WIN32
			virtual ~VulkanWindowSurface();

			VkSurfaceKHR GetHandle() const { return mSurface; }

			VkFormat GetFormat() const { return mFormat; }
			VkColorSpaceKHR GetColorSpace() const { return mColorSpace; }
			const VkSurfaceCapabilitiesKHR GetCapabilities() const;
			VulkanQueueFamily GetPresentQueueFamily() const { return mPresentQueueFamily; }

		private:
			void GetProperties();

			VkSurfaceKHR mSurface;

			VkFormat mFormat;
			VkColorSpaceKHR mColorSpace;
			VulkanQueueFamily mPresentQueueFamily;

			SPtr<VulkanInstance> mInstance_ref;
			SPtr<VulkanPhysicalDevice> mPhysicalDevice_ref;
			SPtr<VulkanDevice> mDevice_ref;
		};
	} // namespace render
} // namespace cube
