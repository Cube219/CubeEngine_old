#pragma once

#include "../VulkanAPIHeader.h"

#include "BaseRenderAPI/Wrapper/Fence.h"

namespace cube
{
	namespace render
	{
		class VULKAN_API_EXPORT VulkanFence : public Fence
		{
		public:
			VulkanFence(SPtr<VulkanDevice>& device);
			virtual ~VulkanFence();

			VkFence GetHandle() const { return mFence; }

			bool Wait(uint64_t timeout) final override;
			void Reset() final override;

		private:
			VkFence mFence;

			SPtr<VulkanDevice> mDevice_ref;
		};
	} // namespace render
} // namespace cube
