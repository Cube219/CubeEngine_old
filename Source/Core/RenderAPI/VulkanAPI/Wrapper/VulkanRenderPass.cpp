#include "VulkanRenderPass.h"

#include "VulkanDevice.h"
#include "VulkanFramebuffer.h"
#include "VulkanImage.h"
#include "VulkanSwapchain.h"

namespace cube
{
	namespace core
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
					PrintLogWithSayer(L"VulkanRenderPass", L"Unknown LoadOperator");
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
					PrintLogWithSayer(L"VulkanRenderPass", L"Unknown StoreOperator");
					break;
			}

			return o;
		}

		UPtr<VulkanSubpass> GetVulkanSubpass(BaseRenderSubpass subpass)
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

		VulkanRenderPass::VulkanRenderPass(const SPtr<VulkanDevice>& device, BaseRenderRenderPassInitializer& initializer) :
			mDevice_ref(device), mSwapchain_ref(nullptr)
		{
		}

		VulkanRenderPass::~VulkanRenderPass()
		{
			vkDestroyRenderPass(mDevice_ref->GetHandle(), mRenderPass, nullptr);

			mFramebuffers.clear();
			mAttachments.clear();
			mSubpasses.clear();
		}

		void VulkanRenderPass::AddAttachment(SPtr<BaseRenderImageView>& imageView, DataFormat format, bool isDepthStencil,
			LoadOperator loadOp, StoreOperator storeOp,
			LoadOperator stencilLoadOp, StoreOperator stencilStoreOp, Color clearColor,
			ImageLayout initialLayout, ImageLayout finalLayout, DepthStencilValue clearDepthStencil)
		{
			mAttachments.push_back(DPCast(VulkanImageView)(imageView));

			VkAttachmentDescription desc;
			desc.flags = 0;
			desc.format = GetVkFormat(format);
			desc.samples = VK_SAMPLE_COUNT_1_BIT; // TODO: 차후 multisampling 구현할 때 수정
			desc.loadOp = GetVkAttachmentLoadOp(loadOp);
			desc.storeOp = GetVkAttachmentStoreOp(storeOp);
			desc.stencilLoadOp = GetVkAttachmentLoadOp(stencilLoadOp);
			desc.stencilStoreOp = GetVkAttachmentStoreOp(stencilStoreOp);
			desc.initialLayout = GetVkImageLayout(initialLayout);
			desc.finalLayout = GetVkImageLayout(finalLayout);

			mAttachmentDescs.push_back(desc);

			VkClearValue clearValue;
			if(isDepthStencil == false) {
				clearValue.color = GetVkClearColorValue(clearColor);
			} else {
				clearValue.depthStencil = GetVkClearDepthStencilValue(clearDepthStencil);
			}
			mAttachmentClearValues.push_back(clearValue);
		}

		void VulkanRenderPass::SetSwapchain(SPtr<BaseRenderSwapchain>& swapchain,
			LoadOperator loadOp, StoreOperator storeOp, Color clearColor,
			ImageLayout initialLayout, ImageLayout finalLayout)
		{
			// TODO: 중복되는 부분 제거?
			mSwapchain_ref = DPCast(VulkanSwapchain)(swapchain);

			// Similar to AddAttachment function, but Swapchain's image views are
			// automatically added to the Framebuffer when the Renderpass is created.
			// So, adding image views in the mAttachments vector is omitted.
			// Also, the swapchain is not used to depth-stencil, omit it's parameters.
			auto& swapchainImageView = mSwapchain_ref->GetImageViews()[0];

			VkAttachmentDescription desc;
			desc.flags = 0;
			desc.format = swapchainImageView->GetVkFormat();
			desc.samples = VK_SAMPLE_COUNT_1_BIT; // TODO: 차후 multisampling 구현할 때 수정
			desc.loadOp = GetVkAttachmentLoadOp(loadOp);
			desc.storeOp = GetVkAttachmentStoreOp(storeOp);
			desc.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			desc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			desc.initialLayout = GetVkImageLayout(initialLayout);
			desc.finalLayout = GetVkImageLayout(finalLayout);

			mAttachmentDescs.push_back(desc);

			VkClearValue clearValue;
			clearValue.color = GetVkClearColorValue(clearColor);
			mAttachmentClearValues.push_back(clearValue);

		}
		void VulkanRenderPass::AddSubpass(BaseRenderSubpass subpass)
		{
			mSubpasses.push_back(GetVulkanSubpass(subpass));
		}

		void VulkanRenderPass::Create()
		{
			VkResult res;

			VkSubpassDescription* subpassDescriptions = new VkSubpassDescription[mSubpasses.size()];
			for(size_t i = 0; i < mSubpasses.size(); i++) {
				subpassDescriptions[i] = mSubpasses[i]->GetDescription();
			}

			VkRenderPassCreateInfo info = {};
			info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
			info.pNext = nullptr;
			info.flags = 0;
			info.attachmentCount = SCast(uint32_t)(mAttachmentDescs.size());
			info.pAttachments = mAttachmentDescs.data();
			info.subpassCount = SCast(uint32_t)(mSubpasses.size());
			info.pSubpasses = subpassDescriptions;
			info.dependencyCount = 0;
			info.pDependencies = nullptr;

			res = vkCreateRenderPass(mDevice_ref->GetHandle(), &info, nullptr, &mRenderPass);
			CheckVkResult(L"VulkanRenderPass", L"Cannot create a VulkanRenderPass", res);

			delete[] subpassDescriptions;

			// TODO: swapchain을 안 쓰는 경우 추가 (framebuffer를 1개만 씀)
			// Create framebuffers to be compatible with the attachments
			auto pThis = shared_from_this();
			auto swapchainImageViews = mSwapchain_ref->GetImageViews();

			uint32_t framebufferCount;
			framebufferCount = SCast(uint32_t)(swapchainImageViews.size());

			mFramebuffers.resize(framebufferCount);
			for(uint32_t i = 0; i < framebufferCount; i++) {
				VulkanFramebufferInitializer framebufferInit;
				framebufferInit.renderPass = pThis;
				framebufferInit.width = mSwapchain_ref->GetWidth();
				framebufferInit.height = mSwapchain_ref->GetHeight();
				framebufferInit.layers = 1;

				framebufferInit.attachments.push_back(swapchainImageViews[i]->GetHandle());
				for(uint32_t j = 0; j < mAttachments.size(); j++) {
					framebufferInit.attachments.push_back(mAttachments[j]->GetHandle());
				}
				mFramebuffers[i] = std::make_shared<VulkanFramebuffer>(mDevice_ref, framebufferInit);
			}
		}

		SPtr<VulkanFramebuffer> VulkanRenderPass::GetFramebuffer()
		{
			if(mSwapchain_ref == nullptr)
				return mFramebuffers[0];
			else
				return mFramebuffers[mSwapchain_ref->GetCurrentImageIndex()];
		}
	}
}