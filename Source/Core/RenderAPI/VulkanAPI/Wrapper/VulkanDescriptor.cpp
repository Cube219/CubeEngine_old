#include "VulkanDescriptor.h"

#include "VulkanDevice.h"
#include "VulkanBuffer.h"
#include "VulkanImage.h"
#include "VulkanSampler.h"

namespace cube
{
	namespace core
	{
		VulkanDescriptorPool::VulkanDescriptorPool() :
			mDescriptorMaxSize(0)
		{
		}

		VulkanDescriptorPool::~VulkanDescriptorPool()
		{
			mDescriptorPoolSizes.clear();

			vkDestroyDescriptorPool(*mDevice_ref, mDescriptorPool, nullptr);
		}

		void VulkanDescriptorPool::AddDescriptorTypeToAllocate(VkDescriptorType type, uint32_t count)
		{
			VkDescriptorPoolSize poolSize;
			poolSize.type = type;
			poolSize.descriptorCount = count;

			mDescriptorMaxSize += count;

			mDescriptorPoolSizes.push_back(poolSize);
		}

		void VulkanDescriptorPool::Create(const SPtr<VulkanDevice>& device)
		{
			mDevice_ref = device;

			VkResult res;

			VkDescriptorPoolCreateInfo info = {};
			info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
			info.pNext = nullptr;
			info.flags = 0;
			info.maxSets = mDescriptorMaxSize;
			info.poolSizeCount = SCast(uint32_t)(mDescriptorPoolSizes.size());
			info.pPoolSizes = mDescriptorPoolSizes.data();

			res = vkCreateDescriptorPool(*device, &info, nullptr, &mDescriptorPool);
			CheckVkResult(L"VulkanDescriptorPool", L"Cannot create a DescriptorPool", res);
		}

		// ------------------------------------------------
		//               VulkanDescriptorSet
		// ------------------------------------------------

		VulkanDescriptorSet::VulkanDescriptorSet(const SPtr<VulkanDevice>& device, const SPtr<VulkanDescriptorPool>& pool) :
			mDevice_ref(device), mDescriptorPool_ref(pool)
		{
		}

		VulkanDescriptorSet::~VulkanDescriptorSet()
		{
			mLayoutBindings.clear();

			vkDestroyDescriptorSetLayout(*mDevice_ref, mLayout, nullptr);
			vkFreeDescriptorSets(*mDevice_ref, *mDescriptorPool_ref, 1, &mDescriptorSet);
		}

		void VulkanDescriptorSet::AddDescriptor(ShaderType shaderType, DescriptorType descriptorType, uint32_t bindingIndex, uint32_t count)
		{
			VkDescriptorSetLayoutBinding b;
			b.binding = bindingIndex;
			b.descriptorType = GetVkDescriptorType(descriptorType);
			b.descriptorCount = count;
			b.stageFlags = GetVkShaderStageFlags(shaderType);
			b.pImmutableSamplers = nullptr;

			mLayoutBindings.push_back(b);
		}

		void VulkanDescriptorSet::Create()
		{
			VkResult res;

			// Create set layout
			VkDescriptorSetLayoutCreateInfo layoutInfo = {};
			layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
			layoutInfo.pNext = nullptr;
			layoutInfo.flags = 0;
			layoutInfo.bindingCount = SCast(uint32_t)(mLayoutBindings.size());
			layoutInfo.pBindings = mLayoutBindings.data();

			res = vkCreateDescriptorSetLayout(*mDevice_ref, &layoutInfo, nullptr, &mLayout);
			CheckVkResult(L"VulkanDescriptorSet", L"Cannot create a layout", res);

			// Allocate set from the pool
			VkDescriptorSetAllocateInfo descriptorSetAllocateInfos = {};
			descriptorSetAllocateInfos.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			descriptorSetAllocateInfos.pNext = nullptr;
			descriptorSetAllocateInfos.descriptorPool = *mDescriptorPool_ref;
			descriptorSetAllocateInfos.descriptorSetCount = 1;
			descriptorSetAllocateInfos.pSetLayouts = &mLayout;

			res = vkAllocateDescriptorSets(*mDevice_ref, &descriptorSetAllocateInfos, &mDescriptorSet);
			CheckVkResult(L"VulkanDescriptorSet", L"Cannot create a VulkanDescriptorSet", res);
		}

		void VulkanDescriptorSet::WriteBufferInDescriptor(uint32_t bindingIndex, uint32_t bufferNum, BaseRenderBufferInfo* buffers)
		{
			VkDescriptorBufferInfo* bufInfos = new VkDescriptorBufferInfo[bufferNum];
			for(uint32_t i = 0; i < bufferNum; i++) {
				bufInfos[i].buffer = *SPCast(VulkanBuffer)(buffers[i].buffer.lock());
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

			vkUpdateDescriptorSets(*mDevice_ref, 1, &writeDescriptorSet, 0, nullptr);

			delete[] bufInfos;
		}

		void VulkanDescriptorSet::WriteImagesInDescriptor(uint32_t bindingIndex, uint32_t imageNum, SPtr<BaseRenderImageView>* imageViews, SPtr<BaseRenderSampler>* samplers)
		{
			VkDescriptorImageInfo* imageInfos = new VkDescriptorImageInfo[imageNum];
			for(uint32_t i = 0; i < imageNum; i++) {
				imageInfos[i].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
				imageInfos[i].imageView = *DPCast(VulkanImageView)(imageViews[i]);
				imageInfos[i].sampler = *DPCast(VulkanSampler)(samplers[i]);
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

			vkUpdateDescriptorSets(*mDevice_ref, 1, &writeDescriptorSet, 0, nullptr);

			delete[] imageInfos;
		}

		VkShaderStageFlags VulkanDescriptorSet::GetVkShaderStageFlags(ShaderType shaderType)
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

		VkDescriptorType VulkanDescriptorSet::GetVkDescriptorType(DescriptorType descType)
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
	}
}