#ifndef VULKAN_LOGICAL_DEVICE_H
#define VULKAN_LOGICAL_DEVICE_H

#include "VulkanAPIHeader.h"

#include "VkObject.h"

namespace cube
{
	namespace render
	{
		class VulkanLogicalDevice : public std::enable_shared_from_this<VulkanLogicalDevice>
		{
		public:
			VulkanLogicalDevice(VulkanPhysicalDevice& physicalDevice, const VkPhysicalDeviceFeatures& enabledFeatures,
				const DeviceAttribute& attr);
			~VulkanLogicalDevice();

			VkDevice GetHandle() const { return mDevice; }

			VulkanPhysicalDevice& GetParentPhysicalDevice() const { return mParentPhysicalDevice; }

			VkBufferWrapper CreateVkBufferWrapper(const VkBufferCreateInfo& info, const char* debugName = "");
			VkBufferViewWrapper CreateVkBufferViewWrapper(const VkBufferViewCreateInfo& info, const char* debugName = "");
			VkImageWrapper CreateVkImageWrapper(const VkImageCreateInfo& info, const char* debugName = nullptr);
			VkImageViewWrapper CreateVkImageViewWrapper(const VkImageViewCreateInfo& info, const char* debugName = "");
			VkFenceWrapper CreateVkFenceWrapper(const VkFenceCreateInfo& info, const char* debugName = "");
			VkSwapChainWrapper CreateVkSwapChainWrapper(const VkSwapchainCreateInfoKHR& info, const char* debugName = "");
			VkPipelineWrapper CreateVkPipelineWrapper(const VkGraphicsPipelineCreateInfo& info, VkPipelineCache cache, const char* debugName = "");
			VkPipelineWrapper CreateVkPipelineWrapper(const VkComputePipelineCreateInfo& info, VkPipelineCache cache, const char* debugName = "");
			VkPipelineLayoutWrapper CreateVkPipelineLayoutWrapper(const VkPipelineLayoutCreateInfo& info, const char* debugName = "");
			VkShaderModuleWrapper CreateVkShaderModuleWrapper(const VkShaderModuleCreateInfo& info, const char* debugName = "");
			VkRenderPassWrapper CreateVkRenderPassWrapper(const VkRenderPassCreateInfo& info, const char* debugName = "");
			VkFramebufferWrapper CreateVkFramebufferWrapper(const VkFramebufferCreateInfo& info, const char* debugName = "");
			VkDescriptorPoolWrapper CreateVkDescriptorPoolWrapper(const VkDescriptorPoolCreateInfo& info, const char* debugName = "");
			VkDescriptorSetLayoutWrapper CreateVkDescriptorSetLayoutWrapper(const VkDescriptorSetLayoutCreateInfo& info, const char* debugName = "");
			VkSamplerWrapper CreateVkSamplerWrapper(const VkSamplerCreateInfo& info, const char* debugName = "");

			void ReleaseVkObject(VkBufferWrapper&& buffer) const;
			void ReleaseVkObject(VkBufferViewWrapper&& bufView) const;
			void ReleaseVkObject(VkImageWrapper&& image) const;
			void ReleaseVkObject(VkImageViewWrapper&& imgView) const;
			void ReleaseVkObject(VkFenceWrapper&& fence) const;
			void ReleaseVkObject(VkSwapChainWrapper&& swapChain) const;
			void ReleaseVkObject(VkPipelineWrapper&& pipeline) const;
			void ReleaseVkObject(VkPipelineLayoutWrapper&& pipelineLayout) const;
			void ReleaseVkObject(VkShaderModuleWrapper&& shaderModule) const;
			void ReleaseVkObject(VkRenderPassWrapper&& renderPass) const;
			void ReleaseVkObject(VkFramebufferWrapper&& framebuffer) const;
			void ReleaseVkObject(VkDescriptorPoolWrapper&& descriptorPool) const;
			void ReleaseVkObject(VkDescriptorSetLayoutWrapper&& descriptorSetLayout) const;
			void ReleaseVkObject(VkSamplerWrapper&& sampler) const;

		private:
			VkDevice mDevice;

			VulkanPhysicalDevice& mParentPhysicalDevice;
		};
	} // namespace render
} // namespace cube

#endif // VULKAN_LOGICAL_DEVICE_H
