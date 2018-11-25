#pragma once

#include "../VulkanAPIHeader.h"

#include "BaseRenderAPI_New/Interface/RenderTarget.h"

namespace cube
{
	namespace render
	{
		class RenderTargetVk final : public RenderTarget
		{
		public:
			RenderTargetVk(const RenderTargetAttribute& attr);
			virtual ~RenderTargetVk();

			VkAttachmentDescription GetVkAttachmentDescription() const { return mAttachmentDesc; }

			bool HasSwapChain() const { return mHasSwapChain; }
			SPtr<TextureViewVk> GetTextureViewVk() const { return mTextureView; }
			SPtr<SwapChainVk> GetSwapChainVk() const { return mSwapChain; }

		private:
			VkAttachmentDescription mAttachmentDesc;
			SPtr<TextureViewVk> mTextureView;
			bool mHasSwapChain;
			SPtr<SwapChainVk> mSwapChain;
		};
	} // namespace render
} // namespace cube
