#pragma once

#include "../VulkanAPIHeader.h"

#include "BaseRenderAPI/Wrapper/BaseRenderImage.h"

namespace cube
{
	namespace core
	{
		VkImageType GetVkImageType(ImageType type);
		VkImageUsageFlags GetVkImageUsageFlags(ImageUsageBits usage);
		VkImageViewType GetVkImageViewType(ImageViewType type);
		VkImageAspectFlags GetVkImageAspectFlags(ImageAspectBits aspect);
		VkImageLayout GetVkImageLayout(ImageLayout imageLayout);

		class VULKAN_API_EXPORT VulkanImageView : public BaseRenderImageView
		{
		public:
			VulkanImageView(const SPtr<VulkanDevice>& device, const SPtr<VulkanImage>& image,
				VkFormat format, VkComponentMapping components, VkImageSubresourceRange subresourceRange,
				VkImageViewType viewType);
			virtual ~VulkanImageView();

			VkImageView GetHandle() const { return mImageView; }

			VkFormat GetVkFormat() const;

		private:
			VkImageView mImageView;

			VkFormat mFormat;

			SPtr<VulkanDevice> mDevice_ref;
		};

		class VULKAN_API_EXPORT VulkanImage : public BaseRenderImage, public std::enable_shared_from_this<VulkanImage>
		{
		public:
			VulkanImage(const SPtr<VulkanDevice>& device, VkImage image);
			VulkanImage(const SPtr<VulkanDevice>& device, BaseRenderImageInitializer& initializer);
			virtual ~VulkanImage();

			VkImage GetHandle() const { return mImage; }

			SPtr<BaseRenderImageView> GetImageView(DataFormat format, ImageAspectBits aspectBits, ImageViewType type) final override;

		private:
			VkImage mImage;
			VkDeviceMemory mAllocatedMemory;

			SPtr<VulkanDevice> mDevice_ref;
		};
	}
}