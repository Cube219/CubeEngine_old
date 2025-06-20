#pragma once

#include "../VulkanAPIHeader.h"

#include "RenderAPI/Interface/RenderTarget.h"

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
			VkClearValue GetDefaultClearValue() const { return mDefaultClearValue; }

		private:
			VkAttachmentDescription mAttachmentDesc;

			SPtr<TextureViewVk> mTextureView;
			bool mHasSwapChain;
			SPtr<SwapChainVk> mSwapChain;

			VkClearValue mDefaultClearValue;
		};
	} // namespace render
} // namespace cube
