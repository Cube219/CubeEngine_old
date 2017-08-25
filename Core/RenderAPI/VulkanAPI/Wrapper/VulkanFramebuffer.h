#pragma once

#include "..\VulkanAPIHeader.h"

namespace cube
{
	namespace core
	{
		class VulkanFramebuffer
		{
		public:
			VulkanFramebuffer();
			~VulkanFramebuffer();

			operator VkFramebuffer() const
			{
				return mFramebuffer;
			}

			void AddAttachment(VkImageView imageView);

			void Create(const SPtr<VulkanDevice>& device, const SPtr<VulkanRenderPass>& renderPass,
				uint32_t width, uint32_t height, uint32_t layers);

		private:
			VkFramebuffer mFramebuffer;

			Vector<VkImageView> mAttachments;

			SPtr<VulkanDevice> mDevice_ref;
		};
	}
}