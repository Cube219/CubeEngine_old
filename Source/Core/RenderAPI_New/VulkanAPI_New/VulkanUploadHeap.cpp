#include "VulkanUploadHeap.h"

#include "VulkanMemoryManager.h"

namespace cube
{
	namespace render
	{
		VulkanUploadHeap::VulkanUploadHeap(VulkanMemoryManager& memoryManager, VkDeviceSize pageSize) : 
			mMemoryManager(memoryManager),
			mDefaultPageSize(pageSize)
		{
		}

		VulkanUploadHeap::~VulkanUploadHeap()
		{
		}

		VulkanUploadAllocation VulkanUploadHeap::Allocate(VkDeviceSize size, VkDeviceSize alignment)
		{
			return VulkanUploadAllocation();
		}

		void VulkanUploadHeap::FinishFrame()
		{
		}

		VulkanUploadHeap::UploadPage VulkanUploadHeap::CreateNewPage(VkDeviceSize size)
		{
			return UploadPage();
		}
	} // namespace render
} // namespace cube
