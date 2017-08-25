#pragma once

#include "..\VulkanAPIHeader.h"

#include <SPIRV\GlslangToSpv.h>

#include "BaseRenderAPI\Wrapper\BaseRenderShader.h"

namespace cube
{
	namespace core
	{
		class VULKAN_API_EXPORT VulkanShader : public BaseRenderShader
		{
		public:
			VulkanShader(const SPtr<VulkanDevice>& device, VkShaderStageFlagBits stageFlagBits, const char* GLSLText, const char* entryName);
			virtual ~VulkanShader();

			operator VkShaderModule() const
			{
				return mShaderModule;
			}

			const VkPipelineShaderStageCreateInfo GetStageInfo() const;

		private:
			TBuiltInResource InitResource();

			VkShaderModule mShaderModule;
			VkPipelineShaderStageCreateInfo mShaderStageInfo;

			Vector<unsigned int> mSpvShader;

			SPtr<VulkanDevice> mDevice_ref;
		};

		inline const VkPipelineShaderStageCreateInfo VulkanShader::GetStageInfo() const
		{
			return mShaderStageInfo;
		}
	}
}