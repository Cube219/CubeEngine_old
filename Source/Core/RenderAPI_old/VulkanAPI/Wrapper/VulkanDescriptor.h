#pragma once

#include "../VulkanAPIHeader.h"

#include "BaseRenderAPI/Wrapper/Descriptor.h"

namespace cube
{
	namespace render
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

		class VULKAN_API_EXPORT VulkanDescriptorSetLayout : public DescriptorSetLayout
		{
		public:
			VulkanDescriptorSetLayout(const SPtr<VulkanDevice>& device, DescriptorSetInitializer& initializer);
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

		class VULKAN_API_EXPORT VulkanDescriptorSet : public DescriptorSet
		{
		public:
			VulkanDescriptorSet(const SPtr<VulkanDevice>& device, const SPtr<VulkanDescriptorPool>& pool, SPtr<DescriptorSetLayout>& layout);
			virtual ~VulkanDescriptorSet();

			VkDescriptorSet GetHandle() const { return mDescriptorSet; }

			void WriteBufferInDescriptor(uint32_t bindingIndex, uint32_t bufferNum, BufferInfo* buffers) final override;
			void WriteImagesInDescriptor(uint32_t bindingIndex, uint32_t imageNum, SPtr<ImageView>* imageViews, SPtr<Sampler>* samplers) final override;

		private:
			VkDescriptorSet mDescriptorSet;

			Vector<VkDescriptorType> mDescriptorTypes;

			SPtr<VulkanDevice> mDevice_ref;
			SPtr<VulkanDescriptorPool> mDescriptorPool_ref;
		};
	} // namespace render
} // namespace cube
