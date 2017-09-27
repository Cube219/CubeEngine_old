#pragma once

#include "..\VulkanAPIHeader.h"

#include "BaseRenderAPI\Wrapper\BaseRenderRenderPass.h"

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
			VulkanRenderPass(const SPtr<VulkanDevice>& device);
			virtual ~VulkanRenderPass();
			VulkanRenderPass(VulkanRenderPass& rhs) = delete;
			VulkanRenderPass& operator=(VulkanRenderPass& rhs) = delete;

			operator VkRenderPass() const
			{
				return mRenderPass;
			}

			void AddAttachment(SPtr<BaseRenderImageView>& imageView, DataFormat format, bool isDepthStencil,
				LoadOperator loadOp, StoreOperator storeOp,
				LoadOperator stencilLoadOp, StoreOperator stencilStoreOp, Color clearColor,
				ImageLayout initialLayout, ImageLayout finalLayout, DepthStencilValue clearDepthStencil) override;

			void SetSwapchain(SPtr<BaseRenderSwapchain>& swapchain,
				LoadOperator loadOp, StoreOperator storeOp, Color clearColor,
				ImageLayout initialLayout, ImageLayout finalLayout) override;

			void AddSubpass(BaseRenderSubpass subpass) override;

			//void SetSwapchain(SPtr<VulkanSwapchain>& swapchain);

			void Create() override;

			SPtr<VulkanFramebuffer> GetFramebuffer();

		private:
			VkRenderPass mRenderPass;

			Vector<SPtr<VulkanFramebuffer>> mFramebuffers;

			Vector<SPtr<VulkanImageView>> mAttachments;
			Vector<VkAttachmentDescription> mAttachmentDescs;
			Vector<VkClearValue> mAttachmentClearValues;
			
			Vector<UPtr<VulkanSubpass>> mSubpasses;

			SPtr<VulkanDevice> mDevice_ref;
			SPtr<VulkanSwapchain> mSwapchain_ref;
		};
	}
}