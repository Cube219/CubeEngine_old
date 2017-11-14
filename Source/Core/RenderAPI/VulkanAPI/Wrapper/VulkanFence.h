#pragma once

#include "../VulkanAPIHeader.h"

#include "BaseRenderAPI/Wrapper/BaseRenderFence.h"

namespace cube
{
	namespace core
	{
		class VULKAN_API_EXPORT VulkanFence : public BaseRenderFence
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
	}
}