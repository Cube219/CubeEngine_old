#pragma once

#include "../VulkanAPIHeader.h"

#include "RenderAPI/Interface/Shader.h"

#include "../VkObject.h"

namespace cube
{
	namespace render
	{
		class ShaderVk final : public Shader
		{
		public:
			ShaderVk(DeviceVk& device, const ShaderAttribute& attr);
			virtual ~ShaderVk();

			VkShaderModule GetModule() const { return mShaderModule.mObject; }
			VkPipelineShaderStageCreateInfo GetPipelineShaderStageInfo() const { return mShaderStageInfo; }

		private:
			void LoadFromGLSL(const ShaderAttribute& attr);
			void LoadFromHLSL(const ShaderAttribute& attr);
			void LoadFromSPIRV(const ShaderAttribute& attr);

			VkShaderModuleWrapper mShaderModule;
			VkPipelineShaderStageCreateInfo mShaderStageInfo;

			Vector<Uint32> mSPIRV;
			std::string mEntryPoint;
		};
	} // namespace render
} // namespace cube
