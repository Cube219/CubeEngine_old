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

		private:
			VkAttachmentDescription mAttachmentDesc;
		};
	} // namespace render
} // namespace cube
