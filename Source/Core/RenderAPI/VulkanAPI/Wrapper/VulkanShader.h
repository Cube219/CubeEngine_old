#pragma once

#include "../VulkanAPIHeader.h"

#include <SPIRV/GlslangToSpv.h>

#include "BaseRenderAPI/Wrapper/BaseRenderShader.h"

namespace cube
{
	namespace core
	{
		class VULKAN_API_EXPORT VulkanShader : public BaseRenderShader
		{
		public:
			VulkanShader(const SPtr<VulkanDevice>& device, BaseRenderShaderInitializer& initializer);
			virtual ~VulkanShader();

			VkShaderModule GetHandle() const { return mShaderModule; }

			const VkPipelineShaderStageCreateInfo GetStageInfo() const { return mShaderStageInfo; }

		private:
			TBuiltInResource InitResource();

			VkShaderModule mShaderModule;
			VkPipelineShaderStageCreateInfo mShaderStageInfo;
			String mEntryName;

			Vector<unsigned int> mSpvShader;

			SPtr<VulkanDevice> mDevice_ref;
		};
	}
}