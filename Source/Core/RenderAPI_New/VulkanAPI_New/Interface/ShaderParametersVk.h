#pragma once

#include "../VulkanAPIHeader.h"

#include "BaseRenderAPI_New/Interface/ShaderParameters.h"

#include "../VulkanShaderParameterManager.h"

namespace cube
{
	namespace render
	{
		// TODO: RawData / Sampler / StorageImage 처리

		class ShaderParametersVk final : public ShaderParameters
		{
		public:
			ShaderParametersVk(VulkanShaderParameterManager& parameterManager, ShaderParametersLayoutVk& layout,
				Vector<VulkanShaderParameterAllocation>& parameterAllocations);
			virtual ~ShaderParametersVk();

			virtual void UpdateParameter(Uint32 bindIndex, void* pData, Uint32 size) override final;

			const Vector<VulkanShaderParameterAllocation>& GetParameterAllocations() const { return mParameterAllocations; }

		private:
			VulkanShaderParameterAllocation& FindAllocation(Uint32 bindIndex);

			VulkanShaderParameterManager& mParameterManager;

			ShaderParametersLayoutVk& mLayout;

			Vector<ShaderParameterInfo> mParameterInfos;
			Vector<VulkanShaderParameterAllocation> mParameterAllocations;
		};
	} // namespace render
} // namespace cube
