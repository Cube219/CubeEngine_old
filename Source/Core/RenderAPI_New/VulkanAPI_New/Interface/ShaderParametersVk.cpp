#include "ShaderParametersVk.h"

#include "ShaderParametersLayoutVk.h"
#include "EngineCore/Assertion.h"

namespace cube
{
	namespace render
	{
		ShaderParametersVk::ShaderParametersVk(VulkanShaderParameterManager& parameterManager, ShaderParametersLayoutVk& layout,
			Vector<VulkanShaderParameterAllocation>& parameterAllocations) :
			mParameterManager(parameterManager), mLayout(layout), mParameterAllocations(parameterAllocations),
			mParameterInfos(layout.GetParameterInfos())
		{
		}

		ShaderParametersVk::~ShaderParametersVk()
		{
			for(auto& alloc : mParameterAllocations) {
				mParameterManager.Free(std::move(alloc));
			}
		}

		void ShaderParametersVk::UpdateParameter(Uint32 bindIndex, void* pData, Uint32 size)
		{
			auto& alloc = FindAllocation(bindIndex);

			// RawData is already allocated even it is PerFrame
			if(alloc.type != ShaderParameterType::RawData && alloc.isPerFrame) {
				alloc = mParameterManager.AllocatePerFrame(alloc.type, alloc.size);
				alloc.bindIndex = bindIndex;
			}

			CHECK(alloc.size == size,
				"Data size doesn't match between allocated size({0}) and update size({1}).", alloc.size, size);

			memcpy(alloc.pData, pData, size);
		}

		VulkanShaderParameterAllocation& ShaderParametersVk::FindAllocation(Uint32 bindIndex)
		{
			for(auto& alloc : mParameterAllocations) {
				if(alloc.bindIndex == bindIndex) {
					return alloc;
				}
			}

			ASSERTION_FAILED("Cannot find allocation whose bind index is '{0}'.", bindIndex);
			return mParameterAllocations[0];
		}
	} // namespace render
} // namespace cube
