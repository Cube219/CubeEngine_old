#pragma once

#include "../VulkanAPIHeader.h"

#include "RenderAPI/Interface/ShaderParameters.h"

#include "../VulkanShaderParameterManager.h"

namespace cube
{
	namespace render
	{
		// TODO: RawData / Sampler / StorageImage 처리

		class ShaderParametersVk final : public ShaderParameters
		{
		public:
			ShaderParametersVk(VkDevice& device, VulkanShaderParameterManager& parameterManager, ShaderParametersLayoutVk& layout,
				const char* debugName);
			virtual ~ShaderParametersVk();

			virtual void UpdateParameter(Uint32 bindIndex, void* pData, Uint32 size) override final;
			virtual void UpdateParameter(Uint32 bindIndex, SPtr<TextureView>& textureView) override final;
			virtual void UpdateParameter(Uint32 bindIndex, SPtr<Sampler>& sampler) override final;
			virtual void UpdateParameter(Uint32 bindIndex, SPtr<TextureView>& textureView, SPtr<Sampler>& sampler) override final;

			const Vector<VulkanShaderParameterAllocation>& GetParameterAllocations() const { return mParameterAllocations; }

			void BindParameters(VkCommandBuffer cmdBuf, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout pipelineLayout,
				Uint32 parametersIndex);

		private:
			VulkanShaderParameterAllocation& FindAllocation(Uint32 bindIndex);

			VulkanShaderParameterManager& mParameterManager;
			VkDevice mDevice;

			ShaderParametersLayoutVk& mLayout;

			VkDescriptorSet mDescriptorSet;
			ShaderParameterInfoList mParameterList;
			Vector<VulkanShaderParameterAllocation> mParameterAllocations;
		};
	} // namespace render
} // namespace cube
