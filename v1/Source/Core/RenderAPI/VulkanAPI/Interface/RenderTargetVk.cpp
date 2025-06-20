#include "RenderTargetVk.h"

#include "TextureViewVk.h"
#include "TextureVk.h"
#include "SwapChainVk.h"
#include "../VulkanTypeConversion.h"

namespace cube
{
	namespace render
	{
		RenderTargetVk::RenderTargetVk(const RenderTargetAttribute& attr) : 
			RenderTarget(attr)
		{
			mAttachmentDesc.flags = 0;
			mAttachmentDesc.format = VK_FORMAT_UNDEFINED;
			mAttachmentDesc.samples = VK_SAMPLE_COUNT_1_BIT; // TODO: MultiSampling 구현 때 구현
			mAttachmentDesc.loadOp = LoadOperatorToVkAttachmentLoadOp(attr.loadOp);
			mAttachmentDesc.storeOp = StoreOperatorToVkAttachmentStoreOp(attr.storeOp);
			mAttachmentDesc.stencilLoadOp = LoadOperatorToVkAttachmentLoadOp(attr.stencilLoadOp);
			mAttachmentDesc.stencilStoreOp = StoreOperatorToVkAttachmentStoreOp(attr.stencilStoreOp);
			mAttachmentDesc.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			mAttachmentDesc.finalLayout = VK_IMAGE_LAYOUT_UNDEFINED;

			mHasSwapChain = attr.isSwapChain;
			if(attr.isSwapChain == false) {
				SPtr<TextureViewVk> textureViewVk = DPCast(TextureViewVk)(attr.textureView);
				mTextureView = textureViewVk;

				mAttachmentDesc.format = textureViewVk->GetVkFormat();

				VkImageUsageFlags usage = textureViewVk->GetParentImage().GetUsage();

				if((usage & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT) > 0) {
					mAttachmentDesc.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

					mDefaultClearValue.color.float32[0] = attr.defaultColor[0];
					mDefaultClearValue.color.float32[1] = attr.defaultColor[1];
					mDefaultClearValue.color.float32[2] = attr.defaultColor[2];
					mDefaultClearValue.color.float32[3] = attr.defaultColor[3];
				} else if((usage & VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT) > 0) {
					mAttachmentDesc.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

					mDefaultClearValue.depthStencil.depth = attr.defaultDepth;
					mDefaultClearValue.depthStencil.stencil = attr.defaultStencil;
				}
			} else {
				SPtr<SwapChainVk> swapChainVk = DPCast(SwapChainVk)(attr.swapChain);
				mSwapChain = swapChainVk;

				mDefaultClearValue.color.float32[0] = attr.defaultColor[0];
				mDefaultClearValue.color.float32[1] = attr.defaultColor[1];
				mDefaultClearValue.color.float32[2] = attr.defaultColor[2];
				mDefaultClearValue.color.float32[3] = attr.defaultColor[3];

				mAttachmentDesc.format = swapChainVk->GetColorVkFormat();
				mAttachmentDesc.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
			}
		}

		RenderTargetVk::~RenderTargetVk()
		{
		}
	} // namespace redner
} // namespace cube
