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

			virtual SPtr<TextureView> CreateView(const TextureViewAttribute& attr) override final;

			VkImageUsageFlags GetUsage() const { return mUsage; }

		private:
			friend class TextureViewVk;

			VkImageWrapper mImage;

			VulkanAllocation mAllocation;

			DeviceVk& mDevice;

			VkImageUsageFlags mUsage;
		};
	} // namespace render
} // namespace cube
