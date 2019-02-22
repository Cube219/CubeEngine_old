#include "VulkanSampler.h"

#include "VulkanDevice.h"

namespace cube
{
	namespace render
	{
		VulkanSampler::VulkanSampler(const SPtr<VulkanDevice>& device) : 
			mDevice_ref(device)
		{
			VkResult res;

			VkSamplerCreateInfo info;
			info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
			info.pNext = nullptr;
			info.flags = 0;
			info.magFilter = VK_FILTER_LINEAR;
			info.minFilter = VK_FILTER_LINEAR;
			info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
			info.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			info.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			info.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			info.mipLodBias = 0;
			info.anisotropyEnable = VK_TRUE;
			info.maxAnisotropy = 16;
			info.compareEnable = VK_FALSE;
			info.compareOp = VK_COMPARE_OP_ALWAYS;
			info.minLod = 0;
			info.maxLod = 0;
			info.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
			info.unnormalizedCoordinates = VK_FALSE;

			res = vkCreateSampler(device->GetHandle(), &info, nullptr, &mSampler);
			CheckVkResult("Cannot create a VulkanSampler", res);
		}

		VulkanSampler::~VulkanSampler()
		{
			vkDestroySampler(mDevice_ref->GetHandle(), mSampler, nullptr);
		}
	} // namespace render
} // namespace cube
