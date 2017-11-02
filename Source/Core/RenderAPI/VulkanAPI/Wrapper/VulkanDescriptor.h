#pragma once

#include "../VulkanAPIHeader.h"

#include "BaseRenderAPI/Wrapper/BaseRenderDescriptor.h"

namespace cube
{
	namespace core
	{
		VkShaderStageFlags GetVkShaderStageFlags(ShaderType shaderType);
		VkDescriptorType GetVkDescriptorType(DescriptorType descType);

		class VulkanDescriptorPool
		{
		public:
			VulkanDescriptorPool(const SPtr<VulkanDevice>& device);
			~VulkanDescriptorPool();

			VkDescriptorPool GetHandle() const { return mDescriptorPool; }

		private:
			VkDescriptorPool mDescriptorPool;

			uint32_t mDescriptorMaxSize;

			SPtr<VulkanDevice> mDevice_ref;
		};

		// ------------------------------------------------
		//               VulkanDescriptorSet
		// ------------------------------------------------

		class VULKAN_API_EXPORT VulkanDescriptorSet : public BaseRenderDescriptorSet
		{
		public:
			VulkanDescriptorSet(const SPtr<VulkanDevice>& device, const SPtr<VulkanDescriptorPool>& pool, BaseRenderDescriptorSetInitializer& initializer);
			virtual ~VulkanDescriptorSet();

			VkDescriptorSet GetHandle() const { return mDescriptorSet; }

			void WriteBufferInDescriptor(uint32_t bindingIndex, uint32_t bufferNum, BaseRenderBufferInfo* buffers) override;
			void WriteImagesInDescriptor(uint32_t bindingIndex, uint32_t imageNum, SPtr<BaseRenderImageView>* imageViews, SPtr<BaseRenderSampler>* samplers) override;

			const VkDescriptorSetLayout GetLayout() const;
			const Vector<VkDescriptorSetLayoutBinding>& GetLayoutBindings() const;

		private:
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