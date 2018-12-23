#include "ShaderParametersVk.h"

#include "ShaderParametersLayoutVk.h"
#include "EngineCore/Assertion.h"

namespace cube
{
	namespace render
	{
		ShaderParametersVk::ShaderParametersVk(VulkanShaderParameterManager& parameterManager, ShaderParametersLayoutVk& layout,
			Vector<VulkanShaderParameterAllocation>& parameterAllocations) :
			mParameterManager(parameterManager), mLayout(layout), mParameterAllocations(parameterAllocations)
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
			auto& alloc = mParameterAllocations[bindIndex];

			if(alloc.isPerFrame) {
				alloc = mParameterManager.AllocatePerFrame(alloc.type, alloc.size);
			}

			CHECK(alloc.size == size,
				"Data size doesn't match between allocated size({0}) and update size({1}).", alloc.size, size);

			memcpy(alloc.pData, pData, size);
		}
	} // namespace render
} // namespace cube
