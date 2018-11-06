#pragma once

#include "VulkanAPIHeader.h"

#include "VulkanMemoryManager.h"

namespace cube
{
	namespace render
	{
		struct VulkanUploadAllocation
		{
		};

		class VulkanUploadHeap
		{
		public:
			VulkanUploadHeap(VulkanMemoryManager& memoryManager, VkDeviceSize pageSize);
			~VulkanUploadHeap();

			VulkanUploadAllocation Allocate(VkDeviceSize size, VkDeviceSize alignment);

			void FinishFrame();

		private:
			struct UploadPage
			{
				VulkanAllocation memAllocation;
				VkBuffer buffer;
			};

			UploadPage CreateNewPage(VkDeviceSize size);

			VulkanMemoryManager& mMemoryManager;

			Vector<UploadPage> mPages;
			VkDeviceSize mDefaultPageSize;
		};
	} // namespace render
} // namespace cube
