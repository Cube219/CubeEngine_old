#pragma once

#include "../VulkanAPIHeader.h"

#include "RenderAPI/Interface/Texture.h"

#include "../VkObject.h"
#include "../VulkanMemoryManager.h"

namespace cube
{
	namespace render
	{
		class TextureVk final : public Texture, public std::enable_shared_from_this<TextureVk>
		{
		public:
			TextureVk(DeviceVk& device, const TextureAttribute& attr,
				VulkanQueueManager& queueManager, VulkanCommandListPool& cmdListPool);
			virtual ~TextureVk();

			VkImage GetHandle() const { return mImage.mObject; }

			virtual void UpdateData(CommandList& cmdList, const void* pData, Uint64 size, Uint32 width, Uint32 height) override final;
			virtual void CopyFromTexture(CommandList& cmdList, Texture& src, Uint32 srcMipLevel, Uint32 srcArrayIndex, const Rect3D* pSrcArea,
				Uint32 dstMipLevel, Uint32 dstArrayIndex, Uint32 dstX, Uint32 dstY, Uint32 dstZ) override final;

			virtual SPtr<TextureView> CreateView(const TextureViewAttribute& attr) override final;

			VkImageUsageFlags GetUsage() const { return mUsage; }
			VkImageLayout GetCurrentLayout() const { return mCurrentLayout; }

		private:
			friend class TextureViewVk;

			void TransitionImageLayout(CommandListVk& cmdList, VkImageLayout newLayout);
			VkAccessFlags GetAccessMask(VkImageLayout layout);
			VkPipelineStageFlags GetPipelineStageFlags(VkImageLayout layout);

			VkImageWrapper mImage;

			VulkanAllocation mAllocation;

			DeviceVk& mDevice;

			VkImageUsageFlags mUsage;
			VkImageLayout mCurrentLayout;
		};
	} // namespace render
} // namespace cube
