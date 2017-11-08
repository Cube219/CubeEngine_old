#pragma once

#include "../VulkanAPIHeader.h"

namespace cube
{
	namespace core
	{
		struct VulkanFramebufferInitializer
		{
			Vector<VkImageView> attachments;
			
			SPtr<VulkanRenderPass> renderPass;
			uint32_t width;
			uint32_t height;
			uint32_t layers;
		};

		class VulkanFramebuffer
		{
		public:
			VulkanFramebuffer(const SPtr<VulkanDevice>& device, VulkanFramebufferInitializer& initializer);
			~VulkanFramebuffer();

			VkFramebuffer GetHandle() const	{ return mFramebuffer; }

		private:
			VkFramebuffer mFramebuffer;

			SPtr<VulkanDevice> mDevice_ref;
		};
	}
}