#include "ShaderParametersVk.h"

#include "ShaderParametersLayoutVk.h"
#include "TextureViewVk.h"
#include "SamplerVk.h"
#include "CommandListVk.h"
#include "EngineCore/Assertion.h"

namespace cube
{
	namespace render
	{
		ShaderParametersVk::ShaderParametersVk(VkDevice& device, VulkanShaderParameterManager& parameterManager, ShaderParametersLayoutVk& layout,
			const char* debugName) :
			ShaderParameters(debugName),
			mDevice(device),
			mParameterManager(parameterManager),
			mLayout(layout),
			mParameterList(layout.GetParameterList())
		{
			mDescriptorSet = parameterManager.AllocateDescriptorSet(layout.GetVkDescriptorSetLayout(), debugName);

			// Write descriptor set to sub-allocated buffers
			// Only uniform / storage buffer be updated
			Vector<VkWriteDescriptorSet> writeDescSets;
			Vector<VkDescriptorBufferInfo> writeBufferInfos;
			// For preventing reallocation
			writeBufferInfos.reserve(mParameterList.parameterInfos.size());

			for(auto& param : mParameterList.parameterInfos) {
				VkWriteDescriptorSet write;
				write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				write.pNext = nullptr;
				write.dstSet = mDescriptorSet;
				write.dstBinding = param.bindIndex;
				write.dstArrayElement = 0;
				write.descriptorCount = 1;

				switch(param.type) {
				case ShaderParameterType::RawData:
					ASSERTION_FAILED("Rawdata type is not implemented.");
					continue;

				case ShaderParameterType::ConstantBuffer:
					writeBufferInfos.push_back({ mParameterManager.GetUniformDynamicBuffer(), 0, VK_WHOLE_SIZE });

					write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
					write.pBufferInfo = &writeBufferInfos.back();
					write.pImageInfo = nullptr;
					write.pTexelBufferView = nullptr;
					break;

				case ShaderParameterType::StorageBuffer:
					writeBufferInfos.push_back({ mParameterManager.GetStorageDynamicBuffer(), 0, VK_WHOLE_SIZE });

					write.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
					write.pBufferInfo = &writeBufferInfos.back();
					write.pImageInfo = nullptr;
					write.pTexelBufferView = nullptr;
					break;

				case ShaderParameterType::ConstantImage:
				case ShaderParameterType::StorageImage:
				case ShaderParameterType::SampledImage:
				case ShaderParameterType::Sampler:
					continue; // Image/Sampler will be updated at UpdateParameter in ShaderParameter

				default:
					ASSERTION_FAILED("Unknown ShaderParameterType ({0})", (int)param.type);
				}

				writeDescSets.push_back(write);
			}

			vkUpdateDescriptorSets(mDevice, SCast(Uint32)(writeDescSets.size()), writeDescSets.data(), 0, nullptr);

			// Allocate parameters (Only uniform/storage buffer)
			for(Uint64 i = 0; i < mParameterList.parameterInfos.size(); i++) {
				auto& info = mParameterList.parameterInfos[i];

				if(info.type == ShaderParameterType::ConstantBuffer ||
					info.type == ShaderParameterType::StorageBuffer) {

					VulkanShaderParameterAllocation alloc;

					if(info.isChangedPerFrame == true) {
						// PerFrame will be allocated in UpdateParameter
						alloc.isPerFrame = true;
						alloc.type = info.type;
						alloc.size = info.size;
					} else {
						alloc = mParameterManager.Allocate(info.type, info.size);
					}
					alloc.bindIndex = info.bindIndex;
					mParameterAllocations.push_back(alloc);
				}
			}
		}

		ShaderParametersVk::~ShaderParametersVk()
		{
			for(auto& alloc : mParameterAllocations) {
				mParameterManager.Free(std::move(alloc));
			}

			mParameterManager.FreeDescriptorSet(mDescriptorSet);
		}

		void ShaderParametersVk::UpdateParameter(Uint32 bindIndex, void* pData, Uint32 size)
		{
			ShaderParameterInfo& info = mParameterList.FindInfo(bindIndex);
			CHECK(info.type == ShaderParameterType::ConstantBuffer ||
				info.type == ShaderParameterType::StorageBuffer,
				"Only Constant(Storage) buffer parameter can be updated via texture view.");

			auto& alloc = FindAllocation(bindIndex);

			if(alloc.isPerFrame) {
				alloc = mParameterManager.AllocatePerFrame(alloc.type, alloc.size);
				alloc.bindIndex = bindIndex;
			}

			CHECK(alloc.size == size,
				"Data size doesn't match between allocated size({0}) and update size({1}).", alloc.size, size);

			memcpy(alloc.pData, pData, size);
		}
		// TODO: perFrame 업데이트 구현
		void ShaderParametersVk::UpdateParameter(Uint32 bindIndex, SPtr<TextureView>& textureView)
		{
			ShaderParameterInfo& info = mParameterList.FindInfo(bindIndex);
			CHECK(info.type == ShaderParameterType::ConstantImage ||
				info.type == ShaderParameterType::StorageImage,
				"Only Constant(Storage) image parameter can be updated via texture view.");

			VkWriteDescriptorSet write;
			write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			write.pNext = nullptr;
			write.dstSet = mDescriptorSet;
			write.dstBinding = bindIndex;
			write.dstArrayElement = 0;
			write.descriptorCount = 1;

			VkDescriptorImageInfo imgInfo;
			imgInfo.sampler = VK_NULL_HANDLE;
			imgInfo.imageView = DPCast(TextureViewVk)(textureView)->GetHandle();

			if(info.type == ShaderParameterType::ConstantImage)
				imgInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			else if(info.type == ShaderParameterType::StorageImage)
				imgInfo.imageLayout = VK_IMAGE_LAYOUT_GENERAL;

			write.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
			write.pImageInfo = &imgInfo;

			vkUpdateDescriptorSets(mDevice, 1, &write, 0, nullptr);
		}

		void ShaderParametersVk::UpdateParameter(Uint32 bindIndex, SPtr<Sampler>& sampler)
		{
			ShaderParameterInfo& info = mParameterList.FindInfo(bindIndex);
			CHECK(info.type == ShaderParameterType::Sampler, "Only Sampler parameter can be updated via sampler.");

			VkWriteDescriptorSet write;
			write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			write.pNext = nullptr;
			write.dstSet = mDescriptorSet;
			write.dstBinding = bindIndex;
			write.dstArrayElement = 0;
			write.descriptorCount = 1;

			VkDescriptorImageInfo imgInfo;
			imgInfo.sampler = DPCast(SamplerVk)(sampler)->GetHandle();
			imgInfo.imageView = VK_NULL_HANDLE;
			imgInfo.imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;

			write.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
			write.pImageInfo = &imgInfo;

			vkUpdateDescriptorSets(mDevice, 1, &write, 0, nullptr);
		}

		void ShaderParametersVk::UpdateParameter(Uint32 bindIndex, SPtr<TextureView>& textureView, SPtr<Sampler>& sampler)
		{
			ShaderParameterInfo& info = mParameterList.FindInfo(bindIndex);
			CHECK(info.type == ShaderParameterType::SampledImage, "Only SampledImage parameter can be updated via sampler and texture view.");

			VkWriteDescriptorSet write;
			write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			write.pNext = nullptr;
			write.dstSet = mDescriptorSet;
			write.dstBinding = bindIndex;
			write.dstArrayElement = 0;
			write.descriptorCount = 1;

			VkDescriptorImageInfo imgInfo;
			imgInfo.sampler = DPCast(SamplerVk)(sampler)->GetHandle();
			imgInfo.imageView = DPCast(TextureViewVk)(textureView)->GetHandle();
			imgInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

			write.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			write.pImageInfo = &imgInfo;

			vkUpdateDescriptorSets(mDevice, 1, &write, 0, nullptr);
		}

		void ShaderParametersVk::BindParameters(VkCommandBuffer cmdBuf, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout pipelineLayout,
			Uint32 parametersIndex)
		{
			Uint64 rawDataAllocationIndex = Uint64InvalidValue;
			Vector<Uint32> dynamicOffsets;
			for(Uint64 i = 0; i < mParameterAllocations.size(); i++) {
				auto& alloc = mParameterAllocations[i];

				// Find dynamic offsets (Constant(Uniform), Storage)
				if((alloc.type == ShaderParameterType::ConstantBuffer) |
					(alloc.type == ShaderParameterType::StorageBuffer)) {
					dynamicOffsets.push_back(SCast(Uint32)(alloc.dynamicOffset));
				}

				if(alloc.type == ShaderParameterType::RawData) {
					rawDataAllocationIndex = i;
				}
			}

			vkCmdBindDescriptorSets(cmdBuf, pipelineBindPoint, pipelineLayout, parametersIndex, 1, &mDescriptorSet,
				SCast(Uint32)(dynamicOffsets.size()), dynamicOffsets.data());
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
