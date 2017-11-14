#pragma once

#include "../VulkanAPIHeader.h"

#include "BaseRenderAPI/Wrapper/BaseRenderRenderPass.h"

namespace cube
{
	namespace core
	{
		VkAttachmentLoadOp GetVkAttachmentLoadOp(LoadOperator loadOperator);
		VkAttachmentStoreOp GetVkAttachmentStoreOp(StoreOperator storeOperator);

		class VULKAN_API_EXPORT VulkanSubpass
		{
		public:
			VulkanSubpass();
			~VulkanSubpass();

			void AddInputAttachmentRef(uint32_t attachmentIndex, VkImageLayout layout);
			void AddColorAttachmentRef(uint32_t attachmentIndex, VkImageLayout layout);
			void SetDepthStencilAttachmentRef(uint32_t attachmentIndex, VkImageLayout layout);

			VkSubpassDescription GetDescription();

		private:
			Vector<VkAttachmentReference> mInputAttachments;
			Vector<VkAttachmentReference> mColorAttachments;
			VkAttachmentReference mDepthStencilAttachment;
		};

		UPtr<VulkanSubpass> GetVulkanSubpass(BaseRenderSubpass subpass);

		// -----------------------------------------------------------
		//                      VulkanRenderPass
		// -----------------------------------------------------------

		class VULKAN_API_EXPORT VulkanRenderPass : public BaseRenderRenderPass, public std::enable_shared_from_this<VulkanRenderPass>
		{
			friend VulkanCommandBuffer;

		public:
			VulkanRenderPass(const SPtr<VulkanDevice>& device, BaseRenderRenderPassInitializer& initializer);
			virtual ~VulkanRenderPass();
			VulkanRenderPass(VulkanRenderPass& rhs) = delete;
			VulkanRenderPass& operator=(VulkanRenderPass& rhs) = delete;

			VkRenderPass GetHandle() const { return mRenderPass; }

			SPtr<VulkanFramebuffer> GetFramebuffer();

		private:
			VkRenderPass mRenderPass;

			Vector<SPtr<VulkanFramebuffer>> mFramebuffers;

			Vector<VkClearValue> mAttachmentClearValues;

			SPtr<VulkanDevice> mDevice_ref;
			SPtr<VulkanSwapchain> mSwapchain_ref;
		};
	}
}