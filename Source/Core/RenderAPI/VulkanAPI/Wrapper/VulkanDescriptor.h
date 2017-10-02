#pragma once

#include "../VulkanAPIHeader.h"

#include "BaseRenderAPI/Wrapper/BaseRenderDescriptor.h"

namespace cube
{
	namespace core
	{
		class VulkanDescriptorPool
		{
		public:
			VulkanDescriptorPool();
			~VulkanDescriptorPool();

			operator VkDescriptorPool() const
			{
				return mDescriptorPool;
			}

			void AddDescriptorTypeToAllocate(VkDescriptorType type, uint32_t count);

			void Create(const SPtr<VulkanDevice>& device);

		private:
			VkDescriptorPool mDescriptorPool;

			Vector<VkDescriptorPoolSize> mDescriptorPoolSizes;
			uint32_t mDescriptorMaxSize;

			SPtr<VulkanDevice> mDevice_ref;
		};

		// ------------------------------------------------
		//               VulkanDescriptorSet
		// ------------------------------------------------

		class VULKAN_API_EXPORT VulkanDescriptorSet : public BaseRenderDescriptorSet
		{
		public:
			VulkanDescriptorSet(const SPtr<VulkanDevice>& device, const SPtr<VulkanDescriptorPool>& pool);
			virtual ~VulkanDescriptorSet();

			operator VkDescriptorSet() const
			{
				return mDescriptorSet;
			}

			void AddDescriptor(ShaderType shaderType, DescriptorType descriptorType, uint32_t bindingIndex, uint32_t count) override;

			void Create() override;

			void WriteBufferInDescriptor(uint32_t bindingIndex, uint32_t bufferNum, BaseRenderBufferInfo* buffers) override;
			void WriteImagesInDescriptor(uint32_t bindingIndex, uint32_t imageNum, SPtr<BaseRenderImageView>* imageViews, SPtr<BaseRenderSampler>* samplers) override;

			/*
					void AddDescriptor(VkDescriptorType type, uint32_t binding, uint32_t count, VkShaderStageFlags stageFlags);

					void WriteBuffer(uint32_t bindingIndex, uint32_t count, const VkDescriptorBufferInfo* pBufferInfos);
					void WriteImage(uint32_t bindingIndex, uint32_t count, const VkDescriptorImageInfo* pImageInfos);
					void WriteTexel(uint32_t bindingIndex, uint32_t count, const VkBufferView* pTexelBufferViews);*/

			const VkDescriptorSetLayout GetLayout() const;
			const Vector<VkDescriptorSetLayoutBinding>& GetLayoutBindings() const;

		private:
			VkShaderStageFlags GetVkShaderStageFlags(ShaderType shaderType);
			VkDescriptorType GetVkDescriptorType(DescriptorType descType);

			VkDescriptorSet mDescriptorSet;

			Vector<VkDescriptorSetLayoutBinding> mLayoutBindings;
			VkDescriptorSetLayout mLayout;

			SPtr<VulkanDevice> mDevice_ref;
			SPtr<VulkanDescriptorPool> mDescriptorPool_ref;
		};

		inline const VkDescriptorSetLayout VulkanDescriptorSet::GetLayout() const
		{
			return mLayout;
		}

		inline const Vector<VkDescriptorSetLayoutBinding>& VulkanDescriptorSet::GetLayoutBindings() const
		{
			return mLayoutBindings;
		}
	}
}