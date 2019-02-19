#include "VulkanLogicalDevice.h"

#include "VulkanUtility.h"
#include "VulkanDebug.h"
#include "VulkanPhysicalDevice.h"
#include "RenderAPI/Interface/Device.h"

#include "EngineCore/LogWriter.h"

namespace cube
{
	namespace render
	{
		VulkanLogicalDevice::VulkanLogicalDevice(VulkanPhysicalDevice& physicalDevice, const VkPhysicalDeviceFeatures& enabledFeatures,
			const DeviceAttribute& attr) : 
			mParentPhysicalDevice(physicalDevice)
		{
			VkResult res;

			Vector<const char*> extensions;
			extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
			extensions.push_back(VK_KHR_DEDICATED_ALLOCATION_EXTENSION_NAME);
			extensions.push_back("VK_KHR_get_memory_requirements2");

			if(attr.enableDebugLayer == true) {
				// Deprecated. Use "VK_EXT_debug_utils" in instance instead.
				// extensions.push_back(VK_EXT_DEBUG_MARKER_EXTENSION_NAME);
			}

			auto& queueFamilyProps = physicalDevice.GetQueueFamilyProperties();

			// Put all queue familiy of the physical device into device queue
			Vector<VkDeviceQueueCreateInfo> queueInfos(queueFamilyProps.size());
			Vector<Vector<float>> queuePriorities(queueFamilyProps.size());

			for(Uint32 i = 0; i < (Uint32)queueFamilyProps.size(); i++) {
				queuePriorities[i].resize(queueFamilyProps[i].queueCount, 1.0f);

				VkDeviceQueueCreateInfo info;
				info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
				info.pNext = nullptr;
				info.flags = 0;
				info.queueFamilyIndex = i;
				info.queueCount = queueFamilyProps[i].queueCount;
				info.pQueuePriorities = queuePriorities[i].data();

				queueInfos[i] = info;
			}

			// Create device
			VkDeviceCreateInfo deviceCreateInfo;
			deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
			deviceCreateInfo.pNext = nullptr;
			deviceCreateInfo.flags = 0;
			deviceCreateInfo.queueCreateInfoCount = SCast(Uint32)(queueInfos.size());
			deviceCreateInfo.pQueueCreateInfos = queueInfos.data();
			deviceCreateInfo.enabledLayerCount = 0; // Deprecated
			deviceCreateInfo.ppEnabledLayerNames = nullptr; // Deprecated
			deviceCreateInfo.enabledExtensionCount = SCast(Uint32)(extensions.size());
			deviceCreateInfo.ppEnabledExtensionNames = extensions.data();
			deviceCreateInfo.pEnabledFeatures = &enabledFeatures;

			res = vkCreateDevice(physicalDevice.GetHandle(), &deviceCreateInfo, nullptr, &mDevice);
			CHECK_VK(res, "Failed to create VkDevice.");

			VulkanDebug::SetObjectName(mDevice, attr.debugName);
		}

		VulkanLogicalDevice::~VulkanLogicalDevice()
		{
			vkDestroyDevice(mDevice, nullptr);
		}

		VkBufferWrapper VulkanLogicalDevice::CreateVkBufferWrapper(const VkBufferCreateInfo& info, const char* debugName)
		{
			VkResult res;

			VkBuffer buf;
			res = vkCreateBuffer(mDevice, &info, nullptr, &buf);
			CHECK_VK(res, "Failed to create VkBuffer '{0}'.", debugName);

			if(debugName != nullptr)
				VulkanDebug::SetObjectName(mDevice, buf, debugName);

			return VkBufferWrapper(buf, shared_from_this(), debugName);
		}

		VkBufferViewWrapper VulkanLogicalDevice::CreateVkBufferViewWrapper(const VkBufferViewCreateInfo& info, const char* debugName)
		{
			VkResult res;

			VkBufferView bufView;
			res = vkCreateBufferView(mDevice, &info, nullptr, &bufView);
			CHECK_VK(res, "Failed to create VkBufferView '{0}'/", debugName);

			if(debugName != nullptr)
				VulkanDebug::SetObjectName(mDevice, bufView, debugName);

			return VkBufferViewWrapper(bufView, shared_from_this(), debugName);
		}

		VkImageWrapper VulkanLogicalDevice::CreateVkImageWrapper(const VkImageCreateInfo& info, const char* debugName)
		{
			VkResult res;

			VkImage image;
			res = vkCreateImage(mDevice, &info, nullptr, &image);
			CHECK_VK(res, "Failed to create VkImage '{0}'/", debugName);

			if(debugName != nullptr)
				VulkanDebug::SetObjectName(mDevice, image, debugName);

			return VkImageWrapper(image, shared_from_this(), debugName);
		}

		VkImageViewWrapper VulkanLogicalDevice::CreateVkImageViewWrapper(const VkImageViewCreateInfo& info, const char* debugName)
		{
			VkResult res;

			VkImageView imgView;
			res = vkCreateImageView(mDevice, &info, nullptr, &imgView);
			CHECK_VK(res, "Failed to create VkImageView '{0}'/", debugName);

			if(debugName != nullptr)
				VulkanDebug::SetObjectName(mDevice, imgView, debugName);

			return VkImageViewWrapper(imgView, shared_from_this(), debugName);
		}

		VkFenceWrapper VulkanLogicalDevice::CreateVkFenceWrapper(const VkFenceCreateInfo& info, const char* debugName)
		{
			VkResult res;

			VkFence fence;
			res = vkCreateFence(mDevice, &info, nullptr, &fence);
			CHECK_VK(res, "Failed to create VkFence '{0}'.", debugName);

			if(debugName != nullptr)
				VulkanDebug::SetObjectName(mDevice, fence, debugName);

			return VkFenceWrapper(fence, shared_from_this(), debugName);
		}

		VkSwapChainWrapper VulkanLogicalDevice::CreateVkSwapChainWrapper(const VkSwapchainCreateInfoKHR& info,
			const char* debugName)
		{
			VkResult res;

			VkSwapchainKHR swapChain;
			res = vkCreateSwapchainKHR(mDevice, &info, nullptr, &swapChain);
			CHECK_VK(res, "Failed to create VkSwapchain '{0}'.", debugName);

			if(debugName != nullptr)
				VulkanDebug::SetObjectName(mDevice, swapChain, debugName);

			return VkSwapChainWrapper(swapChain, shared_from_this(), debugName);
		}

		VkPipelineWrapper VulkanLogicalDevice::CreateVkPipelineWrapper(const VkGraphicsPipelineCreateInfo& info,
			VkPipelineCache cache, const char* debugName)
		{
			VkResult res;

			VkPipeline pipeline;
			res = vkCreateGraphicsPipelines(mDevice, cache, 1, &info, nullptr, &pipeline);
			CHECK_VK(res, "Failed to create VkPipeline for graphics '{0}'.", debugName);

			if(debugName != nullptr)
				VulkanDebug::SetObjectName(mDevice, pipeline, debugName);

			return VkPipelineWrapper(pipeline, shared_from_this(), debugName);
		}

		VkPipelineWrapper VulkanLogicalDevice::CreateVkPipelineWrapper(const VkComputePipelineCreateInfo & info,
			VkPipelineCache cache, const char * debugName)
		{
			VkResult res;

			VkPipeline pipeline;
			res = vkCreateComputePipelines(mDevice, cache, 1, &info, nullptr, &pipeline);
			CHECK_VK(res, "Failed to create VkPipeline for compute '{0}'.", debugName);

			if(debugName != nullptr)
				VulkanDebug::SetObjectName(mDevice, pipeline, debugName);

			return VkPipelineWrapper(pipeline, shared_from_this(), debugName);
		}

		VkPipelineLayoutWrapper VulkanLogicalDevice::CreateVkPipelineLayoutWrapper(const VkPipelineLayoutCreateInfo& info, const char* debugName)
		{
			VkResult res;

			VkPipelineLayout layout;
			res = vkCreatePipelineLayout(mDevice, &info, nullptr, &layout);
			CHECK_VK(res, "Failed to create VkPipelineLayout '{0}'.", debugName);

			if(debugName != nullptr)
				VulkanDebug::SetObjectName(mDevice, layout, debugName);

			return VkPipelineLayoutWrapper(layout, shared_from_this(), debugName);
		}

		VkShaderModuleWrapper VulkanLogicalDevice::CreateVkShaderModuleWrapper(const VkShaderModuleCreateInfo& info,
			const char * debugName)
		{
			VkResult res;

			VkShaderModule shaderModule;
			res = vkCreateShaderModule(mDevice, &info, nullptr, &shaderModule);
			CHECK_VK(res, "Failed to create VkShaderModule '{0}'.", debugName);

			if(debugName != nullptr)
				VulkanDebug::SetObjectName(mDevice, shaderModule, debugName);

			return VkShaderModuleWrapper(shaderModule, shared_from_this(), debugName);
		}

		VkRenderPassWrapper VulkanLogicalDevice::CreateVkRenderPassWrapper(const VkRenderPassCreateInfo& info, const char* debugName)
		{
			VkResult res;

			VkRenderPass renderPass;
			res = vkCreateRenderPass(mDevice, &info, nullptr, &renderPass);
			CHECK_VK(res, "Failed to create VkRenderPass '{0}'.", debugName);

			if(debugName != nullptr)
				VulkanDebug::SetObjectName(mDevice, renderPass, debugName);

			return VkRenderPassWrapper(renderPass, shared_from_this(), debugName);
		}

		VkFramebufferWrapper VulkanLogicalDevice::CreateVkFramebufferWrapper(const VkFramebufferCreateInfo& info, const char* debugName)
		{
			VkResult res;

			VkFramebuffer framebuffer;
			res = vkCreateFramebuffer(mDevice, &info, nullptr, &framebuffer);
			CHECK_VK(res, "Failed to create VkFramebuffer '{0}'.", debugName);

			if(debugName != nullptr)
				VulkanDebug::SetObjectName(mDevice, framebuffer, debugName);

			return VkFramebufferWrapper(framebuffer, shared_from_this(), debugName);
		}

		VkDescriptorPoolWrapper VulkanLogicalDevice::CreateVkDescriptorPoolWrapper(const VkDescriptorPoolCreateInfo& info, const char* debugName)
		{
			VkResult res;

			VkDescriptorPool pool;
			res = vkCreateDescriptorPool(mDevice, &info, nullptr, &pool);
			CHECK_VK(res, "Failed to create VkDescriptorPool '{0}'.", debugName);

			if(debugName != nullptr)
				VulkanDebug::SetObjectName(mDevice, pool, debugName);

			return VkDescriptorPoolWrapper(pool, shared_from_this(), debugName);
		}

		VkDescriptorSetLayoutWrapper VulkanLogicalDevice::CreateVkDescriptorSetLayoutWrapper(const VkDescriptorSetLayoutCreateInfo& info, const char* debugName)
		{
			VkResult res;

			VkDescriptorSetLayout setLayout;
			res = vkCreateDescriptorSetLayout(mDevice, &info, nullptr, &setLayout);
			CHECK_VK(res, "Failed to create VkDescriptorSetLayout '{0}'.", debugName);

			if(debugName != nullptr)
				VulkanDebug::SetObjectName(mDevice, setLayout, debugName);

			return VkDescriptorSetLayoutWrapper(setLayout, shared_from_this(), debugName);
		}

		VkSamplerWrapper VulkanLogicalDevice::CreateVkSamplerWrapper(const VkSamplerCreateInfo& info, const char* debugName)
		{
			VkResult res;

			VkSampler sampler;
			res = vkCreateSampler(mDevice, &info, nullptr, &sampler);
			CHECK_VK(res, "Failed to create VkSampler '{0}'.", debugName);

			if(debugName != nullptr)
				VulkanDebug::SetObjectName(mDevice, sampler, debugName);

			return VkSamplerWrapper(sampler, shared_from_this(), debugName);
		}

		void VulkanLogicalDevice::ReleaseVkObject(VkBufferWrapper&& buffer) const
		{
			vkDestroyBuffer(buffer.GetVkDevice(), buffer.mObject, nullptr);
		}

		void VulkanLogicalDevice::ReleaseVkObject(VkBufferViewWrapper&& bufView) const
		{
			vkDestroyBufferView(bufView.GetVkDevice(), bufView.mObject, nullptr);
		}

		void VulkanLogicalDevice::ReleaseVkObject(VkImageWrapper&& image) const
		{
			vkDestroyImage(image.GetVkDevice(), image.mObject, nullptr);
		}

		void VulkanLogicalDevice::ReleaseVkObject(VkImageViewWrapper&& imgView) const
		{
			vkDestroyImageView(imgView.GetVkDevice(), imgView.mObject, nullptr);
		}

		void VulkanLogicalDevice::ReleaseVkObject(VkFenceWrapper&& fence) const
		{
			vkDestroyFence(fence.GetVkDevice(), fence.mObject, nullptr);
		}

		void VulkanLogicalDevice::ReleaseVkObject(VkSwapChainWrapper&& swapChain) const
		{
			vkDestroySwapchainKHR(swapChain.GetVkDevice(), swapChain.mObject, nullptr);
		}

		void VulkanLogicalDevice::ReleaseVkObject(VkPipelineWrapper&& pipeline) const
		{
			vkDestroyPipeline(mDevice, pipeline.mObject, nullptr);
		}

		void VulkanLogicalDevice::ReleaseVkObject(VkPipelineLayoutWrapper&& pipelineLayout) const
		{
			vkDestroyPipelineLayout(mDevice, pipelineLayout.mObject, nullptr);
		}

		void VulkanLogicalDevice::ReleaseVkObject(VkShaderModuleWrapper&& shaderModule) const
		{
			vkDestroyShaderModule(mDevice, shaderModule.mObject, nullptr);
		}

		void VulkanLogicalDevice::ReleaseVkObject(VkRenderPassWrapper&& renderPass) const
		{
			vkDestroyRenderPass(mDevice, renderPass.mObject, nullptr);
		}

		void VulkanLogicalDevice::ReleaseVkObject(VkFramebufferWrapper&& framebuffer) const
		{
			vkDestroyFramebuffer(mDevice, framebuffer.mObject, nullptr);
		}

		void VulkanLogicalDevice::ReleaseVkObject(VkDescriptorPoolWrapper&& descriptorPool) const
		{
			vkDestroyDescriptorPool(mDevice, descriptorPool.mObject, nullptr);
		}

		void VulkanLogicalDevice::ReleaseVkObject(VkDescriptorSetLayoutWrapper&& descriptorSetLayout) const
		{
			vkDestroyDescriptorSetLayout(mDevice, descriptorSetLayout.mObject, nullptr);
		}

		void VulkanLogicalDevice::ReleaseVkObject(VkSamplerWrapper&& sampler) const
		{
			vkDestroySampler(mDevice, sampler.mObject, nullptr);
		}
	} // namespace render
} // namespace cube
