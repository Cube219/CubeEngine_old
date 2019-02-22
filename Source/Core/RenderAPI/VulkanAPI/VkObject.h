#pragma once

#include "VulkanAPIHeader.h"

namespace cube
{
	namespace render
	{
		template <typename T>
		class VkObject
		{
		public:
			VkObject() : 
				mObject(VK_NULL_HANDLE), mDevice(nullptr), mDebugName("Null VkObject")
			{}
			VkObject(T object, SPtr<VulkanLogicalDevice> device, const char* debugName = "") : 
				mObject(object), mDevice(device), mDebugName(debugName)
			{}
			~VkObject()
			{
				Release();
			}

			VkObject(const VkObject& other) = delete;
			VkObject& operator=(const VkObject& rhs) = delete;

			VkObject(VkObject&& other) :
				mObject(other.mObject),
				mDevice(std::move(other.mDevice)),
				mDebugName(other.mDebugName)
			{
				other.mObject = VK_NULL_HANDLE;
			}
			VkObject& operator=(VkObject&& rhs)
			{
				mObject = rhs.mObject;
				mDevice = std::move(rhs.mDevice);
				mDebugName = rhs.mDebugName;
				
				rhs.mObject = VK_NULL_HANDLE;

				return *this;
			}

			VkDevice GetVkDevice() const { return mDevice->GetHandle(); }
			SPtr<VulkanLogicalDevice> GetVulkanLogicalDevice() const { return mDevice; }

			void Release()
			{
				if(mObject != VK_NULL_HANDLE && mDevice != nullptr) {
					mDevice->ReleaseVkObject(std::move(*this));
				}

				mDevice.reset();
			}

			bool IsEmpty() const
			{
				return mObject == VK_NULL_HANDLE;
			}

			T mObject;
			const char* mDebugName;
	
		private:
			SPtr<VulkanLogicalDevice> mDevice;
		};

		using VkBufferWrapper = VkObject<VkBuffer>;
		using VkBufferViewWrapper = VkObject<VkBufferView>;
		using VkImageWrapper = VkObject<VkImage>;
		using VkImageViewWrapper = VkObject<VkImageView>;
		using VkFenceWrapper = VkObject<VkFence>;
		using VkSwapChainWrapper = VkObject<VkSwapchainKHR>;
		using VkPipelineWrapper = VkObject<VkPipeline>;
		using VkPipelineLayoutWrapper = VkObject<VkPipelineLayout>;
		using VkShaderModuleWrapper = VkObject<VkShaderModule>;
		using VkRenderPassWrapper = VkObject<VkRenderPass>;
		using VkFramebufferWrapper = VkObject<VkFramebuffer>;
		using VkDescriptorPoolWrapper = VkObject<VkDescriptorPool>;
		using VkDescriptorSetLayoutWrapper = VkObject<VkDescriptorSetLayout>;
		using VkDescriptorSetWrapper = VkObject<VkDescriptorSet>;
		using VkSamplerWrapper = VkObject<VkSampler>;

		// -------------------------------------------------
		// Helper class for storaging VkObject child classes
		class VkObjectStorage
		{
		public:
			template <typename VkObjectType>
			static VkObjectStorage Create(VkObjectType&& vkObject)
			{
				class VkObjectWrapperDerivated : public VkObjectWrapperBase
				{
				public:
					VkObjectWrapperDerivated(VkObjectType&& vkObject) :
						mVkObject(std::move(vkObject))
					{}

				private:
					VkObjectType mVkObject;
				};

				return VkObjectStorage(new VkObjectWrapperDerivated(std::move(vkObject)));
			}

		private:
			class VkObjectWrapperBase
			{
			public:
				virtual ~VkObjectWrapperBase() {}
			};

			VkObjectStorage(VkObjectWrapperBase* pWrapperBase) :
				mVkObjectWrapper(pWrapperBase)
			{}

			UPtr<VkObjectWrapperBase> mVkObjectWrapper;
		};
	} // namespace render
} // namespace cube

#ifndef VULKAN_LOGICAL_DEVICE_H
#include "VulkanLogicalDevice.h"
#endif // VULKAN_LOGICAL_DEVICE_H
