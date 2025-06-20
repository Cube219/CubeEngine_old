#pragma once

#include "../VulkanAPIHeader.h"

#include "RenderAPI/Interface/TextureView.h"

#include "../VkObject.h"

namespace cube
{
	namespace render
	{
		class TextureViewVk final : public TextureView
		{
		public:
			TextureViewVk(DeviceVk& device, const TextureViewAttribute& attr, const TextureVk& image);
			virtual ~TextureViewVk();

			VkImageView GetHandle() const { return mImageView.mObject; }
			const TextureVk& GetParentImage() const { return mParentImage; }

			VkFormat GetVkFormat() const { return mFormat; }

		private:
			VkImageViewWrapper mImageView;

			const TextureVk& mParentImage;

			VkFormat mFormat;
		};
	} // namespace render
} // namespace cube
