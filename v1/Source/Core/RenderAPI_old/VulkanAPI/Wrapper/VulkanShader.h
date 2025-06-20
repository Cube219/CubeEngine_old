#pragma once

#include "../VulkanAPIHeader.h"

#include <SPIRV/GlslangToSpv.h>

#include "BaseRenderAPI/Wrapper/Shader.h"

namespace cube
{
	namespace render
	{
		class VULKAN_API_EXPORT VulkanShader : public Shader
		{
		public:
			VulkanShader(const SPtr<VulkanDevice>& device, ShaderInitializer& initializer);
			virtual ~VulkanShader();

			VkShaderModule GetHandle() const { return mShaderModule; }

			const VkPipelineShaderStageCreateInfo GetStageInfo() const { return mShaderStageInfo; }

		private:
			void LoadFromSPIR_V(ShaderInitializer& initializer);
			void LoadFromGLSL(ShaderInitializer& initializer, VkShaderStageFlags stageFlagBits);

			TBuiltInResource InitResource();

			VkShaderModule mShaderModule;
			VkPipelineShaderStageCreateInfo mShaderStageInfo;
			std::string mEntryName;

			Vector<unsigned int> mSpvShader;

			SPtr<VulkanDevice> mDevice_ref;
		};
	} // namespace render
} // namespace cube
