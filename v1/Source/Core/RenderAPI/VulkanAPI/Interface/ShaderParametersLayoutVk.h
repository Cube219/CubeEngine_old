#pragma once

#include "../VulkanAPIHeader.h"

#include "RenderAPI/Interface/ShaderParametersLayout.h"

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

			virtual SPtr<ShaderParameters> CreateParameters() override final;

			bool HasRawDataParameter() const { return mHasRawDataParameter; }
			Uint32 GetRawDataParameterSize() const { return mRawDataParameterSize; }
			const ShaderParameterInfoList& GetParameterList() const { return mParameterList; }

		private:
			friend class ShaderParametersVk;

			VulkanShaderParameterManager& mShaderParameterManager;
			VkDevice mDevice;

			ShaderParameterInfoList mParameterList;

			VkDescriptorSetLayoutWrapper mDescriptorSetLayout;

			bool mHasRawDataParameter = false;
			Uint32 mRawDataParameterSize;
		};
	} // namespace render
} // namespace cube
