#include "VulkanFramebuffer.h"

#include "VulkanRenderPass.h"
#include "VulkanDevice.h"

namespace cube
{
	namespace core
	{
		VulkanFramebuffer::VulkanFramebuffer()
		{
		}

		VulkanFramebuffer::~VulkanFramebuffer()
		{
			vkDestroyFramebuffer(mDevice_ref->GetHandle(), mFramebuffer, nullptr);
		}

		void VulkanFramebuffer::AddAttachment(VkImageView imageView)
		{
			mAttachments.push_back(imageView);
		}

		void VulkanFramebuffer::Create(const SPtr<VulkanDevice>& device, const SPtr<VulkanRenderPass>& renderPass,
			uint32_t width, uint32_t height, uint32_t layers)
		{
			mDevice_ref = device;

			VkResult res;

			VkFramebufferCreateInfo info = {};
			info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			info.pNext = nullptr;
			info.flags = 0;
			info.renderPass = *renderPass;
			info.attachmentCount = SCast(uint32_t)(mAttachments.size());
			info.pAttachments = mAttachments.data();
			info.width = width;
			info.height = height;
			info.layers = layers;

			res = vkCreateFramebuffer(device->GetHandle(), &info, nullptr, &mFramebuffer);
			CheckVkResult(L"VulkanFramebuffer", L"Cannot create a VulkanFramebuffer", res);
		}
	}
}