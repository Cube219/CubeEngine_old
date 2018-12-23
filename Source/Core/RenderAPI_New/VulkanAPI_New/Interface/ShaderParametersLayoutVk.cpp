#include "ShaderParametersLayoutVk.h"

#include "BaseRenderAPI_New/Interface/ShaderParameters.h"

#include "DeviceVk.h"
#include "CommandListVk.h"
#include "ShaderParametersVk.h"
#include "../VulkanLogicalDevice.h"
#include "../VulkanTypeConversion.h"
#include "EngineCore/Assertion.h"

namespace cube
{
	namespace render
	{
		ShaderParametersLayoutVk::ShaderParametersLayoutVk(DeviceVk& device, const ShaderParametersLayoutAttribute& attr) : 
			mShaderParameterManager(device.GetShaderParameterManager()),
			mParameterInfos(attr.paramInfos)
		{
			auto logical = device.GetLogicalDevice();

			VkDescriptorSetLayoutCreateInfo layoutInfo;
			layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
			layoutInfo.pNext = nullptr;
			layoutInfo.flags = 0;

			Uint32 maxBindingNum = 0;
			for(auto& param : attr.paramInfos) {
				if(param.bindIndex > maxBindingNum)
					maxBindingNum = param.bindIndex;
			}

			Vector<VkDescriptorSetLayoutBinding> bindings(maxBindingNum);
			for(auto& param : attr.paramInfos) {
				switch(param.type) {
					case ShaderParameterType::RawData:
						ASSERTION_FAILED("ShaderParameterType::RawData is not implemented yet.");
						break;

					case ShaderParameterType::ConstBuffer:
						bindings[param.bindIndex].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
						break;

					case ShaderParameterType::StorageBuffer:
						bindings[param.bindIndex].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
						break;

					case ShaderParameterType::Sampler:
						ASSERTION_FAILED("ShaderParameterType::Sampler is not implemented yet.");
						bindings[param.bindIndex].descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
						break;

					case ShaderParameterType::StorageImage:
						ASSERTION_FAILED("ShaderParameterType::StorageImage is not implemented yet.");
						bindings[param.bindIndex].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
						break;

					default:
						ASSERTION_FAILED("Unknown ShaderParameterType ({0})", (int)param.type);
				}

				bindings[param.bindIndex].binding = param.bindIndex;
				bindings[param.bindIndex].descriptorCount = param.count;
				bindings[param.bindIndex].stageFlags = VK_SHADER_STAGE_ALL;
				bindings[param.bindIndex].pImmutableSamplers = nullptr;
			}

			layoutInfo.bindingCount = SCast(Uint32)(bindings.size());
			layoutInfo.pBindings = bindings.data();

			mDescriptorSetLayout = logical->CreateVkDescriptorSetLayoutWrapper(layoutInfo, attr.debugName);

			mDescriptorSet = mShaderParameterManager.AllocateDescriptorSet(mDescriptorSetLayout.mObject, mDescriptorSetLayout.mDebugName);

			// Write descriptor set to sub-allocated buffers
			Vector<VkWriteDescriptorSet> writeDescSets;
			Vector<VkDescriptorBufferInfo> writeBufferInfos;
			Vector<VkDescriptorImageInfo> writeImageInfos;
			Vector<VkBufferView> writeTexelBufferViews;

			for(auto& param : attr.paramInfos) {
				VkWriteDescriptorSet write;
				write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				write.pNext = nullptr;
				write.dstSet = mDescriptorSet;
				write.dstBinding = param.bindIndex;
				write.dstArrayElement = 0;
				write.descriptorCount = 1;

				switch(param.type) {
					case ShaderParameterType::RawData:
						ASSERTION_FAILED("ShaderParameterType::RawData is not implemented yet.");
						break; // continue;

					case ShaderParameterType::ConstBuffer:
						writeBufferInfos.push_back({ mShaderParameterManager.GetUniformDynamicBuffer(), 0, VK_WHOLE_SIZE });

						write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
						write.pBufferInfo = &writeBufferInfos.back();
						write.pImageInfo = nullptr;
						write.pTexelBufferView = nullptr;
						break;

					case ShaderParameterType::StorageBuffer:
						writeBufferInfos.push_back({ mShaderParameterManager.GetStorageDynamicBuffer(), 0, VK_WHOLE_SIZE });

						write.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
						write.pBufferInfo = &writeBufferInfos.back();
						write.pImageInfo = nullptr;
						write.pTexelBufferView = nullptr;
						break;

					case ShaderParameterType::Sampler:
						ASSERTION_FAILED("ShaderParameterType::Sampler is not implemented yet.");
						break;

					case ShaderParameterType::StorageImage:
						ASSERTION_FAILED("ShaderParameterType::StorageImage is not implemented yet.");
						break;

					default:
						ASSERTION_FAILED("Unknown ShaderParameterType ({0})", (int)param.type);
				}

				writeDescSets.push_back(write);
			}

			vkUpdateDescriptorSets(mDescriptorSetLayout.GetVkDevice(), SCast(Uint32)(writeDescSets.size()), writeDescSets.data(), 0, nullptr);
		}

		ShaderParametersLayoutVk::~ShaderParametersLayoutVk()
		{
			mShaderParameterManager.FreeDescriptorSet(mDescriptorSet);
		}

		SPtr<ShaderParameters> ShaderParametersLayoutVk::GetParameters()
		{
			Vector<VulkanShaderParameterAllocation> parameterAllocations(mParameterInfos.size());

			for(Uint64 i = 0; i < mParameterInfos.size(); i++) {
				auto& info = mParameterInfos[i];

				if(info.isChangedPerFrame == false) {
					parameterAllocations[i] = mShaderParameterManager.Allocate(info.type, info.size);
				} /* else {
					// PerFrame parameter will be allocated at updating parameter data
				} */
			}

			return std::make_shared<ShaderParametersVk>(mShaderParameterManager, *this, parameterAllocations);
		}

		void ShaderParametersLayoutVk::BindParameters(SPtr<CommandList>& cmdList, PipelineType pipelineType, VkPipelineLayout pipelineLayout,
			Uint32 parametersIndex, ShaderParametersVk& params)
		{
			VkCommandBuffer cmdBuf = DPCast(CommandListVk)(cmdList)->GetHandle();

			VkPipelineBindPoint bindPoint;
			switch(pipelineType) {
				case PipelineType::Graphics: bindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS; break;
				case PipelineType::Compute:  bindPoint = VK_PIPELINE_BIND_POINT_COMPUTE; break;

				default: ASSERTION_FAILED("Unknown PipelineType ({0})", (int)pipelineType);
			}

			auto& parameterAllocations = params.GetParameterAllocations();

			Vector<Uint32> dynamicOffsets;
			for(Uint64 i = 0; i < parameterAllocations.size(); i++) {
				auto& alloc = parameterAllocations[i];

				// Find dynamic offsets (Uniform(Const), Storage)
				if((alloc.type == ShaderParameterType::ConstBuffer) |
					(alloc.type == ShaderParameterType::StorageBuffer)) {
					dynamicOffsets.push_back(SCast(Uint32)(alloc.dynamicOffset));
				}
			}

			vkCmdBindDescriptorSets(cmdBuf, bindPoint, pipelineLayout, parametersIndex, 1, &mDescriptorSet,
				SCast(Uint32)(dynamicOffsets.size()), dynamicOffsets.data());
		}
	} // namespace render
} // namespace cube
