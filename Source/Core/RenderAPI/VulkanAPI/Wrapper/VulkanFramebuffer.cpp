#include "VulkanFramebuffer.h"

#include "VulkanRenderPass.h"
#include "VulkanDevice.h"

namespace cube
{
	namespace core
	{
		VulkanFramebuffer::VulkanFramebuffer(const SPtr<VulkanDevice>& device, VulkanFramebufferInitializer& initializer) : 
			mDevice_ref(device)
		{
			VkResult res;

			VkFramebufferCreateInfo info = {};
			info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			info.pNext = nullptr;
			info.flags = 0;
			info.renderPass = *initializer.renderPass;
			info.attachmentCount = SCast(uint32_t)(initializer.attachments.size());
			info.pAttachments = initializer.attachments.data();
			info.width = initializer.width;
			info.height = initializer.height;
			info.layers = initializer.layers;

			res = vkCreateFramebuffer(device->GetHandle(), &info, nullptr, &mFramebuffer);
			CheckVkResult(L"VulkanFramebuffer", L"Cannot create a VulkanFramebuffer", res);
		}

		VulkanFramebuffer::~VulkanFramebuffer()
		{
			vkDestroyFramebuffer(mDevice_ref->GetHandle(), mFramebuffer, nullptr);
		}
	}
}