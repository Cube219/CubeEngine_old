#include "VulkanRenderPass.h"

#include "EngineCore/Assertion.h"
#include "VulkanDevice.h"
#include "VulkanFramebuffer.h"
#include "VulkanImage.h"
#include "VulkanSwapchain.h"
#include "VulkanGraphicsPipeline.h"

namespace cube
{
	namespace render
	{
		VkAttachmentLoadOp GetVkAttachmentLoadOp(LoadOperator loadOperator)
		{
			VkAttachmentLoadOp o;

			switch(loadOperator) {
				case LoadOperator::Load:
					o = VK_ATTACHMENT_LOAD_OP_LOAD;
					break;
				case LoadOperator::Clear:
					o = VK_ATTACHMENT_LOAD_OP_CLEAR;
					break;
				case LoadOperator::DontCare:
					o = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
					break;

				default:
					ASSERTION_FAILED("Unknown LoadOperator ({0})", (int)loadOperator);
					break;
			}

			return o;
		}

		VkAttachmentStoreOp GetVkAttachmentStoreOp(StoreOperator storeOperator)
		{
			VkAttachmentStoreOp o;

			switch(storeOperator) {
				case StoreOperator::Store:
					o = VK_ATTACHMENT_STORE_OP_STORE;
					break;
				case StoreOperator::DontCare:
					o = VK_ATTACHMENT_STORE_OP_DONT_CARE;
					break;

				default:
					ASSERTION_FAILED("Unknown StoreOperator ({0})", (int)storeOperator);
					break;
			}

			return o;
		}

		UPtr<VulkanSubpass> GetVulkanSubpass(Subpass subpass)
		{
			auto s = std::make_unique<VulkanSubpass>();

			for(auto input : subpass.mInputs) {
				s->AddInputAttachmentRef(input.index, GetVkImageLayout(input.layout));
			}
			for(auto color : subpass.mColors) {
				s->AddColorAttachmentRef(color.index, GetVkImageLayout(color.layout));
			}
			s->SetDepthStencilAttachmentRef(subpass.mDepthStencil.index, GetVkImageLayout(subpass.mDepthStencil.layout));

			return std::move(s);
		}

		VulkanSubpass::VulkanSubpass()
		{
		}

		VulkanSubpass::~VulkanSubpass()
		{
			mInputAttachments.clear();
			mColorAttachments.clear();
		}

		void VulkanSubpass::AddInputAttachmentRef(uint32_t attachmentIndex, VkImageLayout layout)
		{
			VkAttachmentReference ref;
			ref.attachment = attachmentIndex;
			ref.layout = layout;

			mInputAttachments.push_back(ref);
		}

		void VulkanSubpass::AddColorAttachmentRef(uint32_t attachmentIndex, VkImageLayout layout)
		{
			VkAttachmentReference ref;
			ref.attachment = attachmentIndex;
			ref.layout = layout;

			mColorAttachments.push_back(ref);
		}

		void VulkanSubpass::SetDepthStencilAttachmentRef(uint32_t attachmentIndex, VkImageLayout layout)
		{
			mDepthStencilAttachment.attachment = attachmentIndex;
			mDepthStencilAttachment.layout = layout;
		}

		VkSubpassDescription VulkanSubpass::GetDescription()
		{
			VkSubpassDescription desc;
			desc.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
			desc.flags = 0;

			desc.inputAttachmentCount = SCast(uint32_t)(mInputAttachments.size());
			desc.pInputAttachments = mInputAttachments.data();

			desc.colorAttachmentCount = SCast(uint32_t)(mColorAttachments.size());
			desc.pColorAttachments = mColorAttachments.data();
			desc.pResolveAttachments = nullptr;

			desc.pDepthStencilAttachment = &mDepthStencilAttachment;

			desc.preserveAttachmentCount = 0;
			desc.pPreserveAttachments = nullptr;

			return desc;
		}

		// -----------------------------------------------------------
		//                      VulkanRenderPass
		// -----------------------------------------------------------

		VulkanRenderPass::VulkanRenderPass(const SPtr<VulkanDevice>& device, RenderPassInitializer& initializer) :
			mDevice_ref(device), mSwapchain_ref(nullptr)
		{
			VkResult res;

			// Set attachments
			Vector<VkImageView> attachments;
			Vector<VkAttachmentDescription> attachmentDescs;
			attachments.resize(initializer.attachments.size());
			attachmentDescs.resize(initializer.attachments.size());
			mAttachmentClearValues.resize(initializer.attachments.size());
			for(uint64_t i = 0; i < initializer.attachments.size(); i++) {
				auto a = initializer.attachments[i];

				// Attachment
				attachments[i] = DPCast(VulkanImageView)(a.imageView)->GetHandle();

				// AttachmentDesc
				VkAttachmentDescription desc;
				desc.flags = 0;
				desc.format = GetVkFormat(a.format);
				desc.samples = VK_SAMPLE_COUNT_1_BIT; // TODO: 차후 multisampling 구현할 때 수정
				desc.loadOp = GetVkAttachmentLoadOp(a.loadOp);
				desc.storeOp = GetVkAttachmentStoreOp(a.storeOp);
				desc.stencilLoadOp = GetVkAttachmentLoadOp(a.stencilLoadOp);
				desc.stencilStoreOp = GetVkAttachmentStoreOp(a.stencilStoreOp);
				desc.initialLayout = GetVkImageLayout(a.initialLayout);
				desc.finalLayout = GetVkImageLayout(a.finalLayout);
				attachmentDescs[i] = desc;

				// Clear value
				VkClearValue clearValue;
				if(a.isDepthStencil == false) {
					clearValue.color = GetVkClearColorValue(a.clearColor);
				} else {
					clearValue.depthStencil = GetVkClearDepthStencilValue(a.clearDepthStencil);
				}
				mAttachmentClearValues[i] = clearValue;
			}
			
			// Set attachment for swapchain
			if(initializer.hasSwapchain == true) {
				mSwapchain_ref = DPCast(VulkanSwapchain)(initializer.swapchainAttachment.swapchain);

				auto& swapchainImageView = mSwapchain_ref->GetImageViews()[0];

				VkAttachmentDescription desc;
				desc.flags = 0;
				desc.format = swapchainImageView->GetVkFormat();
				desc.samples = VK_SAMPLE_COUNT_1_BIT; // TODO: 차후 multisampling 구현할 때 수정
				desc.loadOp = GetVkAttachmentLoadOp(initializer.swapchainAttachment.loadOp);
				desc.storeOp = GetVkAttachmentStoreOp(initializer.swapchainAttachment.storeOp);
				desc.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				desc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
				desc.initialLayout = GetVkImageLayout(initializer.swapchainAttachment.initialLayout);
				desc.finalLayout = GetVkImageLayout(initializer.swapchainAttachment.finalLayout);
				attachmentDescs.push_back(desc);

				VkClearValue clearValue;
				clearValue.color = GetVkClearColorValue(initializer.swapchainAttachment.clearColor);
				mAttachmentClearValues.push_back(clearValue);
			}

			// Set subpass
			Vector<UPtr<VulkanSubpass>> subpassesWrapper; // Temporary used
			Vector<VkSubpassDescription> subpasses;
			subpassesWrapper.resize(initializer.subpasses.size());
			subpasses.resize(initializer.subpasses.size());
			for(uint64_t i = 0; i < initializer.subpasses.size(); i++) {
				subpassesWrapper[i] = GetVulkanSubpass(initializer.subpasses[i]);
				subpasses[i] = subpassesWrapper[i]->GetDescription();
			}
			Vector<VkSubpassDependency> subpassDependencies;
			subpassDependencies.resize(initializer.subpassDependencies.size());
			for(uint64_t i = 0; i < initializer.subpassDependencies.size(); i++) {
				auto d = initializer.subpassDependencies[i];

				subpassDependencies[i].srcSubpass = d.srcIndex;
				subpassDependencies[i].dstSubpass = d.dstIndex;
				subpassDependencies[i].srcStageMask = GetVkPipelineStageFlags(d.srcStageMask);
				subpassDependencies[i].dstAccessMask = GetVkPipelineStageFlags(d.dstStageMask);
				subpassDependencies[i].srcAccessMask = GetVkAccessFlags(d.srcAccessMask);
				subpassDependencies[i].dstAccessMask = GetVkAccessFlags(d.dstAccessMask);
				subpassDependencies[i].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
			}

			VkRenderPassCreateInfo info = {};
			info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
			info.pNext = nullptr;
			info.flags = 0;
			info.attachmentCount = SCast(uint32_t)(attachmentDescs.size());
			info.pAttachments = attachmentDescs.data();
			info.subpassCount = SCast(uint32_t)(subpasses.size());
			info.pSubpasses = subpasses.data();
			info.dependencyCount = SCast(uint32_t)(subpassDependencies.size());
			info.pDependencies = subpassDependencies.data();

			res = vkCreateRenderPass(mDevice_ref->GetHandle(), &info, nullptr, &mRenderPass);
			CheckVkResult("Cannot create a VulkanRenderPass", res);

			// Create framebuffers to be compatible with the attachments
			uint32_t framebufferCount;
			// If it uses the swapchain, copy framebuffer by the number of swapchain's iamges
			// and put each swapchain's images to the framebuffer
			// Otherwise, just create one framebuffer
			if(initializer.hasSwapchain == false) {
				framebufferCount = 1;
			} else {
				framebufferCount = mSwapchain_ref->GetImageCount();
			}
			mFramebuffers.resize(framebufferCount);
			for(uint32_t i = 0; i < framebufferCount; i++) {
				VulkanFramebufferInitializer framebufferInit;
				framebufferInit.renderPass = this;
				framebufferInit.width = mSwapchain_ref->GetWidth();
				framebufferInit.height = mSwapchain_ref->GetHeight();
				framebufferInit.layers = 1;

				for(uint32_t j = 0; j < attachments.size(); j++) {
					framebufferInit.attachments.push_back(attachments[j]);
				}
				if(initializer.hasSwapchain == true) {
					auto& swapchainImageViews = mSwapchain_ref->GetImageViews();
					framebufferInit.attachments.push_back(swapchainImageViews[i]->GetHandle());
				}

				mFramebuffers[i] = std::make_shared<VulkanFramebuffer>(mDevice_ref, framebufferInit);
			}
		}

		VulkanRenderPass::~VulkanRenderPass()
		{
			mFramebuffers.clear();

			vkDestroyRenderPass(mDevice_ref->GetHandle(), mRenderPass, nullptr);
		}

		SPtr<VulkanFramebuffer> VulkanRenderPass::GetFramebuffer()
		{
			if(mSwapchain_ref == nullptr)
				return mFramebuffers[0];
			else
				return mFramebuffers[mSwapchain_ref->GetCurrentImageIndex()];
		}
	} // namespace render
} // namespace cube
