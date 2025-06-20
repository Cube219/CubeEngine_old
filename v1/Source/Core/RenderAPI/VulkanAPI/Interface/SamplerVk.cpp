#include "SamplerVk.h"

#include "DeviceVk.h"
#include "../VulkanLogicalDevice.h"
#include "../VulkanTypeConversion.h"
#include "EngineCore/Assertion.h"

namespace cube
{
	namespace render
	{
		SamplerVk::SamplerVk(DeviceVk& device, const SamplerAttribute& attr) :
			Sampler(attr)
		{
			VkSamplerCreateInfo info;
			info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
			info.pNext = nullptr;
			info.flags = 0;
			info.magFilter = FilterTypeToVkFilter(attr.magFilter);
			info.minFilter = FilterTypeToVkFilter(attr.minFilter);
			info.mipmapMode = FilterTypeToVkSamplerMipmapMode(attr.mipMapFilter);
			info.addressModeU = AddressModeToVkSamplerAddressMode(attr.addressU);
			info.addressModeV = AddressModeToVkSamplerAddressMode(attr.addressV);
			info.addressModeW = AddressModeToVkSamplerAddressMode(attr.addressW);
			info.mipLodBias = attr.mipLodBias;
			info.anisotropyEnable = IsAnisotropicFilter(attr.minFilter);
			info.maxAnisotropy = SCast(float)(attr.maxAnisotropy);
			info.compareEnable = VK_FALSE; // TODO: 차후 구현
			info.compareOp = VK_COMPARE_OP_ALWAYS;
			info.minLod = attr.minLod;
			info.maxLod = attr.maxLod;
			info.borderColor = GetVkBorderColor(attr.borderColor);
			info.unnormalizedCoordinates = VK_FALSE;

			mSampler = device.GetLogicalDevice()->CreateVkSamplerWrapper(info, attr.debugName);
		}

		SamplerVk::~SamplerVk()
		{
			mSampler.Release();
		}

		bool SamplerVk::IsAnisotropicFilter(FilterType minFilter)
		{
			return minFilter == FilterType::Anisotropy;
		}

		VkBorderColor SamplerVk::GetVkBorderColor(const float borderColor[4])
		{
			if(borderColor[0] == 0.0f && borderColor[1] == 0.0f && borderColor[2] == 0.0f && borderColor[3] == 0.0f)
				return VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;
			else if(borderColor[0] == 0.0f && borderColor[1] == 0.0f && borderColor[2] == 0.0f && borderColor[3] == 1.0f)
				return VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK;
			else if(borderColor[0] == 1.0f && borderColor[1] == 1.0f && borderColor[2] == 1.0f && borderColor[3] == 1.0f)
				return VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;

			ASSERTION_FAILED("In vulkan, border color must be transparent black(0, 0, 0, 0), opaque black(0, 0, 0, 1) or opaque white(1, 1, 1, 1).");

			return VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;
		}
	} // namespace render
} // namespace cube
