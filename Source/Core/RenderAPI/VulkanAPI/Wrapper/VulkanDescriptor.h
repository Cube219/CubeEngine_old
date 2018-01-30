#pragma once

#include "../VulkanAPIHeader.h"

#include "BaseRenderAPI/Wrapper/BaseRenderDescriptor.h"

namespace cube
{
	namespace core
	{
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
		//               VulkanDescriptorSetLayout
		// ------------------------------------------------

		class VULKAN_API_EXPORT VulkanDescriptorSetLayout : public BaseRenderDescriptorSetLayout
		{
		public:
			VulkanDescriptorSetLayout(const SPtr<VulkanDevice>& device, BaseRenderDescriptorSetInitializer& initializer);
			virtual ~VulkanDescriptorSetLayout();

			VkDescriptorSetLayout GetHandle() const { return mDescriptorSetLayout; }
			const Vector<VkDescriptorSetLayoutBinding>& GetBindings() const { return mBindings; }

		private:
			VkDescriptorSetLayout mDescriptorSetLayout;
			Vector<VkDescriptorSetLayoutBinding> mBindings;

			SPtr<VulkanDevice> mDevice_ref;
		};

		// ------------------------------------------------
		//               VulkanDescriptorSet
		// ------------------------------------------------

		class VULKAN_API_EXPORT VulkanDescriptorSet : public BaseRenderDescriptorSet
		{
		public:
			VulkanDescriptorSet(const SPtr<VulkanDevice>& device, const SPtr<VulkanDescriptorPool>& pool, SPtr<BaseRenderDescriptorSetLayout>& layout);
			virtual ~VulkanDescriptorSet();

			VkDescriptorSet GetHandle() const { return mDescriptorSet; }

			void WriteBufferInDescriptor(uint32_t bindingIndex, uint32_t bufferNum, BaseRenderBufferInfo* buffers) final override;
			void WriteImagesInDescriptor(uint32_t bindingIndex, uint32_t imageNum, SPtr<BaseRenderImageView>* imageViews, SPtr<BaseRenderSampler>* samplers) final override;

		private:
			VkDescriptorSet mDescriptorSet;

			Vector<VkDescriptorType> mDescriptorTypes;

			SPtr<VulkanDevice> mDevice_ref;
			SPtr<VulkanDescriptorPool> mDescriptorPool_ref;
		};
	}
}