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
				mObject(VK_NULL_HANDLE), mDevice(nullptr)
			{}
			VkObject(T object, SPtr<VulkanLogicalDevice> device) : 
				mObject(mObject), mDevice(device)
			{}
			~VkObject()
			{
				Release();
			}

			VkObject(const VkObject& other) = delete;
			VkObject& operator=(const VkObject& rhs) = delete;

			VkObject(VkObject&& other) :
				mObject(other.mObject),
				mDevice(std::move(other.mDevice))
			{
				other.mObject = VK_NULL_HANDLE;
			}
			VkObject& operator=(VkObject&& rhs)
			{
				mObject = rhs.mObject;
				mDevice = std::move(rhs.mDevice);
				
				rhs.mObject = VK_NULL_HANDLE;

				return *this;
			}

			VkDevice GetVkDevice() const { return mDevice->GetHandle(); }

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
	
		private:
			SPtr<VulkanLogicalDevice> mDevice;
		};

		using VkBufferWrapper = VkObject<VkBuffer>;
		using VkFenceWrapper = VkObject<VkFence>;

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
