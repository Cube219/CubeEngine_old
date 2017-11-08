#include "VulkanDescriptor.h"

#include "VulkanDevice.h"
#include "VulkanBuffer.h"
#include "VulkanImage.h"
#include "VulkanSampler.h"

namespace cube
{
	namespace core
	{
		VkShaderStageFlags GetVkShaderStageFlags(ShaderType shaderType)
		{
			VkShaderStageFlags shaderStageFlags;

			switch(shaderType) {
				case ShaderType::GLSL_Vertex:
					shaderStageFlags = VK_SHADER_STAGE_VERTEX_BIT;
					break;

				case ShaderType::GLSL_Fragment:
					shaderStageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
					break;

				default:
					PrintLogWithSayer(L"VulkanDescriptorSet", L"Unknown ShaderType");
					break;
			}
			return shaderStageFlags;
		}

		VkDescriptorType GetVkDescriptorType(DescriptorType descType)
		{
			VkDescriptorType t;

			switch(descType) {
				case DescriptorType::Sampler:
					t = VK_DESCRIPTOR_TYPE_SAMPLER;
					break;
				case DescriptorType::CombinedImageSampler:
					t = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
					break;
				case DescriptorType::SampledImage:
					t = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
					break;
				case DescriptorType::StorageImage:
					t = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
					break;
				case DescriptorType::UniformTexelBuffer:
					t = VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER;
					break;
				case DescriptorType::StorageTexelBuffer:
					t = VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER;
					break;
				case DescriptorType::UniformBuffer:
					t = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
					break;
				case DescriptorType::StorageBuffer:
					t = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
					break;
				case DescriptorType::UniformBufferDynamic:
					t = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
					break;
				case DescriptorType::StorageBufferDynamic:
					t = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
					break;
				case DescriptorType::InputAttachment:
					t = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
					break;

				default:
					PrintlnLogWithSayer(L"VulkanDescriptor", L"Unknown DescriptorType");
					break;
			}

			return t;
		}

		VulkanDescriptorPool::VulkanDescriptorPool(const SPtr<VulkanDevice>& device) :
			mDevice_ref(device), mDescriptorMaxSize(0)
		{
			VkResult res;

			auto limits = device->GetProperties().limits;

			uint32_t maxDescriptorSet = 4096;

			Vector<VkDescriptorPoolSize> sizes;
			VkDescriptorPoolSize s;

			s.type = VK_DESCRIPTOR_TYPE_SAMPLER;
			s.descriptorCount = limits.maxDescriptorSetSamplers;
			sizes.push_back(s);

			s.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			s.descriptorCount = limits.maxDescriptorSetSampledImages;
			sizes.push_back(s);

			s.type = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
			s.descriptorCount = limits.maxDescriptorSetSampledImages;
			sizes.push_back(s);

			s.type = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
			s.descriptorCount = limits.maxDescriptorSetStorageImages;
			sizes.push_back(s);

			s.type = VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER;
			s.descriptorCount = limits.maxDescriptorSetUniformBuffers;
			sizes.push_back(s);

			s.type = VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER;
			s.descriptorCount = limits.maxDescriptorSetStorageBuffers;
			sizes.push_back(s);

			s.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			s.descriptorCount = limits.maxDescriptorSetUniformBuffers;
			sizes.push_back(s);

			s.type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
			s.descriptorCount = limits.maxDescriptorSetStorageBuffers;
			sizes.push_back(s);

			s.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
			s.descriptorCount = limits.maxDescriptorSetUniformBuffersDynamic;
			sizes.push_back(s);

			s.type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
			s.descriptorCount = limits.maxDescriptorSetStorageBuffersDynamic;
			sizes.push_back(s);

			s.type = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
			s.descriptorCount = limits.maxDescriptorSetInputAttachments;
			sizes.push_back(s);

			VkDescriptorPoolCreateInfo info = {};
			info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
			info.pNext = nullptr;
			info.flags = 0;
			info.maxSets = maxDescriptorSet;
			info.poolSizeCount = SCast(uint32_t)(sizes.size());
			info.pPoolSizes = sizes.data();

			res = vkCreateDescriptorPool(device->GetHandle(), &info, nullptr, &mDescriptorPool);
			CheckVkResult(L"VulkanDescriptorPool", L"Cannot create a DescriptorPool", res);
		}

		VulkanDescriptorPool::~VulkanDescriptorPool()
		{
			vkDestroyDescriptorPool(mDevice_ref->GetHandle(), mDescriptorPool, nullptr);
		}

		// ------------------------------------------------
		//               VulkanDescriptorSet
		// ------------------------------------------------

		VulkanDescriptorSet::VulkanDescriptorSet(const SPtr<VulkanDevice>& device, const SPtr<VulkanDescriptorPool>& pool, BaseRenderDescriptorSetInitializer& initializer) :
			mDevice_ref(device), mDescriptorPool_ref(pool)
		{
			VkResult res;

			for(uint64_t i = 0; i < initializer.descriptors.size(); i++) {
				auto descriptor = initializer.descriptors[i];

				VkDescriptorSetLayoutBinding b;
				b.binding = descriptor.bindingIndex;
				b.descriptorType = GetVkDescriptorType(descriptor.type);
				b.descriptorCount = descriptor.count;
				b.stageFlags = GetVkShaderStageFlags(descriptor.shaderType);
				b.pImmutableSamplers = nullptr;

				mLayoutBindings.push_back(b);
			}

			// Create set layout
			VkDescriptorSetLayoutCreateInfo layoutInfo = {};
			layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
			layoutInfo.pNext = nullptr;
			layoutInfo.flags = 0;
			layoutInfo.bindingCount = SCast(uint32_t)(mLayoutBindings.size());
			layoutInfo.pBindings = mLayoutBindings.data();

			res = vkCreateDescriptorSetLayout(device->GetHandle(), &layoutInfo, nullptr, &mLayout);
			CheckVkResult(L"VulkanDescriptorSet", L"Cannot create a layout", res);

			// Allocate set from the pool
			VkDescriptorSetAllocateInfo descriptorSetAllocateInfos = {};
			descriptorSetAllocateInfos.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			descriptorSetAllocateInfos.pNext = nullptr;
			descriptorSetAllocateInfos.descriptorPool = pool->GetHandle();
			descriptorSetAllocateInfos.descriptorSetCount = 1;
			descriptorSetAllocateInfos.pSetLayouts = &mLayout;

			res = vkAllocateDescriptorSets(device->GetHandle(), &descriptorSetAllocateInfos, &mDescriptorSet);
			CheckVkResult(L"VulkanDescriptorSet", L"Cannot create a VulkanDescriptorSet", res);
		}

		VulkanDescriptorSet::~VulkanDescriptorSet()
		{
			mLayoutBindings.clear();

			vkDestroyDescriptorSetLayout(mDevice_ref->GetHandle(), mLayout, nullptr);
			vkFreeDescriptorSets(mDevice_ref->GetHandle(), mDescriptorPool_ref->GetHandle(), 1, &mDescriptorSet);
		}

		void VulkanDescriptorSet::WriteBufferInDescriptor(uint32_t bindingIndex, uint32_t bufferNum, BaseRenderBufferInfo* buffers)
		{
			VkDescriptorBufferInfo* bufInfos = new VkDescriptorBufferInfo[bufferNum];
			for(uint32_t i = 0; i < bufferNum; i++) {
				bufInfos[i].buffer = SPCast(VulkanBuffer)(buffers[i].buffer.lock())->GetHandle();
				bufInfos[i].offset = buffers[i].offset;
				bufInfos[i].range = buffers[i].range;
			}

			VkWriteDescriptorSet writeDescriptorSet = {};
			writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			writeDescriptorSet.pNext = nullptr;
			writeDescriptorSet.dstSet = mDescriptorSet;
			writeDescriptorSet.dstBinding = bindingIndex;
			writeDescriptorSet.dstArrayElement = 0;
			writeDescriptorSet.descriptorType = mLayoutBindings[bindingIndex].descriptorType;
			writeDescriptorSet.descriptorCount = bufferNum;
			writeDescriptorSet.pBufferInfo = bufInfos;
			writeDescriptorSet.pImageInfo = nullptr;
			writeDescriptorSet.pTexelBufferView = nullptr;

			vkUpdateDescriptorSets(mDevice_ref->GetHandle(), 1, &writeDescriptorSet, 0, nullptr);

			delete[] bufInfos;
		}

		void VulkanDescriptorSet::WriteImagesInDescriptor(uint32_t bindingIndex, uint32_t imageNum, SPtr<BaseRenderImageView>* imageViews, SPtr<BaseRenderSampler>* samplers)
		{
			VkDescriptorImageInfo* imageInfos = new VkDescriptorImageInfo[imageNum];
			for(uint32_t i = 0; i < imageNum; i++) {
				imageInfos[i].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
				imageInfos[i].imageView = DPCast(VulkanImageView)(imageViews[i])->GetHandle();
				imageInfos[i].sampler = DPCast(VulkanSampler)(samplers[i])->GetHandle();
			}

			VkWriteDescriptorSet writeDescriptorSet = {};
			writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			writeDescriptorSet.pNext = nullptr;
			writeDescriptorSet.dstSet = mDescriptorSet;
			writeDescriptorSet.dstBinding = bindingIndex;
			writeDescriptorSet.dstArrayElement = 0;
			writeDescriptorSet.descriptorType = mLayoutBindings[bindingIndex].descriptorType;
			writeDescriptorSet.descriptorCount = imageNum;
			writeDescriptorSet.pBufferInfo = nullptr;
			writeDescriptorSet.pImageInfo = imageInfos;
			writeDescriptorSet.pTexelBufferView = nullptr;

			vkUpdateDescriptorSets(mDevice_ref->GetHandle(), 1, &writeDescriptorSet, 0, nullptr);

			delete[] imageInfos;
		}
	}
}