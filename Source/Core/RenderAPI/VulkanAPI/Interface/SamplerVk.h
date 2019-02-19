#pragma once

#include "../VulkanAPIHeader.h"

#include "RenderAPI/Interface/Sampler.h"

#include "../VkObject.h"

namespace cube
{
	namespace render
	{
		class SamplerVk final : public Sampler
		{
		public:
			SamplerVk(DeviceVk& device, const SamplerAttribute& attr);
			virtual ~SamplerVk();

			VkSampler GetHandle() const { return mSampler.mObject; }

		private:
			bool IsAnisotropicFilter(FilterType minFilter);
			VkBorderColor GetVkBorderColor(const float borderColor[4]);

			VkSamplerWrapper mSampler;
		};
	} // namespace render
} // namespace cube
