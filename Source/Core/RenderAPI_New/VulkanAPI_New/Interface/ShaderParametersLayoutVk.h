#pragma once

#include "../VulkanAPIHeader.h"

#include "BaseRenderAPI_New/Interface/ShaderParametersLayout.h"

#include "../VkObject.h"

namespace cube
{
	namespace render
	{
		class ShaderParametersLayoutVk final : public ShaderParametersLayout
		{
		public:
			ShaderParametersLayoutVk(DeviceVk& device, const ShaderParametersLayoutAttribute& attr);
			virtual ~ShaderParametersLayoutVk();

			VkDescriptorSetLayout GetVkDescriptorSetLayout() const { return mDescriptorSetLayout.mObject; }

			virtual SPtr<ShaderParameters> GetParameters() override final;

			const Vector<ShaderParameterInfo>& GetParameterInfos() const { return mParameterInfos; }

		private:
			friend class ShaderParametersVk;

			void BindParameters(SPtr<CommandList>& cmdList, PipelineType pipelineType, VkPipelineLayout pipelineLayout,
				Uint32 parametersIndex, ShaderParametersVk& params);

			VulkanShaderParameterManager& mShaderParameterManager;

			Vector<ShaderParameterInfo> mParameterInfos;

			VkDescriptorSetLayoutWrapper mDescriptorSetLayout;
			VkDescriptorSet mDescriptorSet;
		};
	} // namespace render
} // namespace cube
