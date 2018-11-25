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

			VkImageLayout GetVkImageLayout() const { return VK_IMAGE_LAYOUT_GENERAL; } // TODO: 차후 수정
			VkAttachmentDescription GetVkAttachmentDescription() const { return VkAttachmentDescription(); } // TODO: 차후 수정
		};
	} // namespace render
} // namespace cube
