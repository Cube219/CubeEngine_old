#pragma once

#include "../VulkanAPIHeader.h"

#include "BaseRenderAPI_New/Interface/TextureView.h"

#include "../VkObject.h"

namespace cube
{
	namespace render
	{
		class TextureViewVk final : public TextureView
		{
		public:
			TextureViewVk(DeviceVk& device, const TextureViewAttribute& attr, const VkImageWrapper& image);
			virtual ~TextureViewVk();

			VkImageView GetHandle() const { return mImageView.mObject; }

		private:
			VkImageViewWrapper mImageView;
		};
	} // namespace render
} // namespace cube
