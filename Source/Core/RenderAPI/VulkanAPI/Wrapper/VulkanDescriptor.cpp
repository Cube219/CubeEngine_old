#include "VulkanDescriptor.h"

#include "EngineCore/Assertion.h"
#include "VulkanDevice.h"
#include "VulkanBuffer.h"
#include "VulkanImage.h"
#include "VulkanSampler.h"

namespace cube
{
	namespace render
	{
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
					ASSERTION_FAILED("Unknown DescriptorType ({0})", (int)descType);
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
			info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
			info.maxSets = maxDescriptorSet;
			info.poolSizeCount = SCast(uint32_t)(sizes.size());
			info.pPoolSizes = sizes.data();

			res = vkCreateDescriptorPool(device->GetHandle(), &info, nullptr, &mDescriptorPool);
			CheckVkResult("Cannot create a DescriptorPool", res);
		}

		VulkanDescriptorPool::~VulkanDescriptorPool()
		{
			vkDestroyDescriptorPool(mDevice_ref->GetHandle(), mDescriptorPool, nullptr);
		}

		// ------------------------------------------------
		//               VulkanDescriptorSetLayout
		// ------------------------------------------------

		VulkanDescriptorSetLayout::VulkanDescriptorSetLayout(const SPtr<VulkanDevice>& device, DescriptorSetInitializer& initializer) : 
			mDevice_ref(device)
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

				mBindings.push_back(b);
			}

			VkDescriptorSetLayoutCreateInfo layoutInfo = {};
			layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
			layoutInfo.pNext = nullptr;
			layoutInfo.flags = 0;
			layoutInfo.bindingCount = SCast(uint32_t)(mBindings.size());
			layoutInfo.pBindings = mBindings.data();

			res = vkCreateDescriptorSetLayout(device->GetHandle(), &layoutInfo, nullptr, &mDescriptorSetLayout);
			CheckVkResult("Cannot create a layout", res);
		}

		VulkanDescriptorSetLayout::~VulkanDescriptorSetLayout()
		{
			mBindings.clear();

			vkDestroyDescriptorSetLayout(mDevice_ref->GetHandle(), mDescriptorSetLayout, nullptr);
		}

		// ------------------------------------------------
		//               VulkanDescriptorSet
		// ------------------------------------------------

		VulkanDescriptorSet::VulkanDescriptorSet(const SPtr<VulkanDevice>& device, const SPtr<VulkanDescriptorPool>& pool, SPtr<DescriptorSetLayout>& layout) :
			mDevice_ref(device), mDescriptorPool_ref(pool)
		{
			VkResult res;

			// Allocate set from the pool
			VkDescriptorSetLayout vkLayout = SPCast(VulkanDescriptorSetLayout)(layout)->GetHandle();

			VkDescriptorSetAllocateInfo descriptorSetAllocateInfos = {};
			descriptorSetAllocateInfos.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			descriptorSetAllocateInfos.pNext = nullptr;
			descriptorSetAllocateInfos.descriptorPool = pool->GetHandle();
			descriptorSetAllocateInfos.descriptorSetCount = 1;
			descriptorSetAllocateInfos.pSetLayouts = &vkLayout;

			res = vkAllocateDescriptorSets(device->GetHandle(), &descriptorSetAllocateInfos, &mDescriptorSet);
			CheckVkResult("Cannot create a VulkanDescriptorSet", res);

			auto bindings = SPCast(VulkanDescriptorSetLayout)(layout)->GetBindings();
			mDescriptorTypes.resize(bindings.size());

			for(uint32_t i = 0; i < bindings.size(); i++) {
				mDescriptorTypes[i] = bindings[i].descriptorType;
			}
		}

		VulkanDescriptorSet::~VulkanDescriptorSet()
		{
			mDescriptorTypes.clear();
			
			vkFreeDescriptorSets(mDevice_ref->GetHandle(), mDescriptorPool_ref->GetHandle(), 1, &mDescriptorSet);
		}

		void VulkanDescriptorSet::WriteBufferInDescriptor(uint32_t bindingIndex, uint32_t bufferNum, BufferInfo* buffers)
		{
			VkDescriptorBufferInfo* bufInfos = new VkDescriptorBufferInfo[bufferNum];
			for(uint32_t i = 0; i < bufferNum; i++) {
				bufInfos[i].buffer = SPCast(const VulkanBuffer)(buffers[i].buffer)->GetHandle();
				bufInfos[i].offset = buffers[i].offset;
				bufInfos[i].range = buffers[i].range;
			}

			VkWriteDescriptorSet writeDescriptorSet = {};
			writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			writeDescriptorSet.pNext = nullptr;
			writeDescriptorSet.dstSet = mDescriptorSet;
			writeDescriptorSet.dstBinding = bindingIndex;
			writeDescriptorSet.dstArrayElement = 0;
			writeDescriptorSet.descriptorType = mDescriptorTypes[bindingIndex];
			writeDescriptorSet.descriptorCount = bufferNum;
			writeDescriptorSet.pBufferInfo = bufInfos;
			writeDescriptorSet.pImageInfo = nullptr;
			writeDescriptorSet.pTexelBufferView = nullptr;

			vkUpdateDescriptorSets(mDevice_ref->GetHandle(), 1, &writeDescriptorSet, 0, nullptr);

			delete[] bufInfos;
		}

		void VulkanDescriptorSet::WriteImagesInDescriptor(uint32_t bindingIndex, uint32_t imageNum, SPtr<ImageView>* imageViews, SPtr<Sampler>* samplers)
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
			writeDescriptorSet.descriptorType = mDescriptorTypes[bindingIndex];
			writeDescriptorSet.descriptorCount = imageNum;
			writeDescriptorSet.pBufferInfo = nullptr;
			writeDescriptorSet.pImageInfo = imageInfos;
			writeDescriptorSet.pTexelBufferView = nullptr;

			vkUpdateDescriptorSets(mDevice_ref->GetHandle(), 1, &writeDescriptorSet, 0, nullptr);

			delete[] imageInfos;
		}
	} // namespace render
} // namespace cube
