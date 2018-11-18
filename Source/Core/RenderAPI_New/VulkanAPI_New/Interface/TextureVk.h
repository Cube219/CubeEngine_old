#pragma once

#include "../VulkanAPIHeader.h"

#include "BaseRenderAPI_New/Interface/Texture.h"

#include "../VkObject.h"
#include "../VulkanMemoryManager.h"

namespace cube
{
	namespace render
	{
		class TextureVk final : public Texture
		{
		public:
			TextureVk(DeviceVk& device, const TextureAttribute& attr,
				VulkanQueueManager& queueManager, VulkanCommandListPool& cmdListPool);
			virtual ~TextureVk();

			virtual void UpdateData(CommandList& cmdList, const void* pData, Uint64 size, Uint32 width, Uint32 height) override final;

		private:
			VkImageWrapper mImage;

			VulkanAllocation mAllocation;
		};
	} // namespace render
} // namespace cube
