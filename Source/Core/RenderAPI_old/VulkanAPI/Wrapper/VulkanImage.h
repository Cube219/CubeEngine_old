#pragma once

#include "../VulkanAPIHeader.h"

#include "BaseRenderAPI/Wrapper/Image.h"

namespace cube
{
	namespace render
	{
		VkImageType GetVkImageType(ImageType type);
		VkImageUsageFlags GetVkImageUsageFlags(ImageUsageBits usage);
		VkImageViewType GetVkImageViewType(ImageViewType type);
		VkImageAspectFlags GetVkImageAspectFlags(ImageAspectBits aspect);
		VkImageLayout GetVkImageLayout(ImageLayout imageLayout);

		class VULKAN_API_EXPORT VulkanImageView : public ImageView
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

		class VULKAN_API_EXPORT VulkanImage : public Image, public std::enable_shared_from_this<VulkanImage>
		{
		public:
			VulkanImage(const SPtr<VulkanDevice>& device, VkImage image);
			VulkanImage(const SPtr<VulkanDevice>& device, ImageInitializer& initializer);
			virtual ~VulkanImage();

			VkImage GetHandle() const { return mImage; }

			SPtr<ImageView> GetImageView(DataFormat format, ImageAspectBits aspectBits, ImageViewType type) final override;

		private:
			VkImage mImage;
			VkDeviceMemory mAllocatedMemory;

			SPtr<VulkanDevice> mDevice_ref;
		};
	} // namespace render
} // namespace cube
