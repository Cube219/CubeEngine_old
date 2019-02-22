#include "VulkanShaderParameterManager.h"

#include "VulkanDebug.h"
#include "VulkanUtility.h"
#include "VulkanLogicalDevice.h"
#include "VulkanPhysicalDevice.h"
#include "Interface/DeviceVk.h"

#include "EngineCore/Assertion.h"

namespace cube
{
	namespace render
	{
		///////////////////////////////
		// VulkanShaderParameterHeap //
		///////////////////////////////

		VulkanShaderParameterHeap::VulkanShaderParameterHeap(SPtr<VulkanLogicalDevice>& device, VulkanMemoryManager& memManager,
			Uint32 heapIndex, ShaderParameterType type) : 
			mHeapIndex(heapIndex), mHeapType(type)
		{
			VkResult res;

			auto& limits = device->GetParentPhysicalDevice().GetProperties().limits;

			VkBufferCreateInfo bufInfo;
			bufInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			bufInfo.pNext = nullptr;
			bufInfo.flags = 0;
			bufInfo.queueFamilyIndexCount = 0;
			bufInfo.pQueueFamilyIndices = nullptr;
			bufInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
			
			Uint32 bufSize;
			const char* bufDebugName = "";
			switch(type) {
				case ShaderParameterType::ConstantBuffer:
					bufSize = (UniformBufferMaxSize > limits.maxUniformBufferRange) ? limits.maxUniformBufferRange : UniformBufferMaxSize;
					bufInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
					bufDebugName = "Uniform buffer in ShaderParameterHeap";
					mMinAlignment = limits.minUniformBufferOffsetAlignment;
					break;

				case ShaderParameterType::StorageBuffer:
					bufSize = (StorageBufferMaxSize > limits.maxStorageBufferRange) ? limits.maxStorageBufferRange : StorageBufferMaxSize;
					bufInfo.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
					bufDebugName = "Storage buffer in ShaderParameterHeap";
					mMinAlignment = limits.minStorageBufferOffsetAlignment;
					break;

				case ShaderParameterType::RawData:
					ASSERTION_FAILED("RawData type is stored in VulkanShaderParameterManager. Not ShaderParameterHeap.");
					break;

				case ShaderParameterType::ConstantImage:
				case ShaderParameterType::StorageImage:
				case ShaderParameterType::SampledImage:
				case ShaderParameterType::Sampler:
					ASSERTION_FAILED("Image/Sampler cannot be allocated in VulkanShaderParamterHeap.");
					break;

				default:
					ASSERTION_FAILED("Unknown ShaderParameterType ({0})", (Uint32)type);
			}
			bufInfo.size = bufSize;

			mBuffer = device->CreateVkBufferWrapper(bufInfo, bufDebugName);

			VkMemoryRequirements memRequire;
			vkGetBufferMemoryRequirements(mBuffer.GetVkDevice(), mBuffer.mObject, &memRequire);

			mBufferMemoryAllocation = memManager.Allocate(memRequire, MemoryUsage::CPUtoGPU);

			res = vkBindBufferMemory(mBuffer.GetVkDevice(), mBuffer.mObject,
				mBufferMemoryAllocation.deviceMemory, mBufferMemoryAllocation.offset);
			CHECK_VK(res, "Failed to bind buffer memory.");

			mStartOffset = mBufferMemoryAllocation.offset;
			mEndOffset = mBufferMemoryAllocation.offset + mBufferMemoryAllocation.size;
			mPageForNonPerFrame = VariableSizeMemoryPage(mBufferMemoryAllocation.size / 2, mStartOffset);

			mStartFrameOffset = mStartOffset + (mBufferMemoryAllocation.size / 2);
			mCurrentFrameOffset = mStartFrameOffset;
		}

		VulkanShaderParameterHeap::~VulkanShaderParameterHeap()
		{
			mBufferMemoryAllocation.Free();
			mBuffer.Release();
		}

		VulkanShaderParameterAllocation VulkanShaderParameterHeap::Allocate(Uint64 size)
		{
			VariableSizeMemoryPage::Allocation alloc = mPageForNonPerFrame.Allocate(size, mMinAlignment);

			VulkanShaderParameterAllocation allocation;
			allocation.type = mHeapType;
			allocation.isPerFrame = false;
			allocation.buffer = mBuffer.mObject;
			allocation.pData = (char*)mBufferMemoryAllocation.mappedData + alloc.offset;
			allocation.size = size;
			allocation.dynamicOffset = alloc.offset;
			allocation.dynamicUnalignedOffset = alloc.unalignedOffset;

			return allocation;
		}

		VulkanShaderParameterAllocation VulkanShaderParameterHeap::AllocatePerFrame(Uint64 size)
		{
			Uint64 alignedOffset = Align(mCurrentFrameOffset, mMinAlignment);
			CHECK(alignedOffset + size < mEndOffset, "No space in {0} (Per frame).", mBuffer.mDebugName);

			VulkanShaderParameterAllocation allocation;
			allocation.type = mHeapType;
			allocation.isPerFrame = true;
			allocation.buffer = mBuffer.mObject;
			allocation.pData = (char*)mBufferMemoryAllocation.mappedData + alignedOffset;
			allocation.size = size;
			allocation.dynamicOffset = alignedOffset;
			allocation.dynamicUnalignedOffset = mCurrentFrameOffset;

			mCurrentFrameOffset = alignedOffset + size;

			return allocation;
		}

		void VulkanShaderParameterHeap::Free(VulkanShaderParameterAllocation&& allocation)
		{
			// This function will never have PerFrame allocation
			// because the allocation will be skipped in VulkanShaderParameterManager::Free
			// So, skip checking PerFrame
			/*
			if(allocation.isPerFrame)
				return;
			*/

			VariableSizeMemoryPage::Allocation alloc{allocation.dynamicOffset,
				allocation.dynamicUnalignedOffset, allocation.size};

			mPageForNonPerFrame.Free(alloc);
		}

		void VulkanShaderParameterHeap::DiscardPerFrame()
		{
			mCurrentFrameOffset = mStartFrameOffset;
		}

		//////////////////////////////////
		// VulkanShaderParameterManager //
		//////////////////////////////////

		VulkanShaderParameterManager::VulkanShaderParameterManager(SPtr<VulkanLogicalDevice>& device, VulkanMemoryManager& memManager) :
			mUniformHeap{device, memManager, 0, ShaderParameterType::ConstantBuffer},
			mStorageHeap{device, memManager, 1, ShaderParameterType::StorageBuffer}
		{
			auto& limits = device->GetParentPhysicalDevice().GetProperties().limits;

			VkDescriptorPoolCreateInfo descInfo;
			descInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
			descInfo.pNext = nullptr;
			descInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
			descInfo.maxSets = 4096;

			Vector<VkDescriptorPoolSize> poolSize
			{
				{VK_DESCRIPTOR_TYPE_SAMPLER,                limits.maxDescriptorSetSamplers},
				{VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, limits.maxDescriptorSetSampledImages},
				{VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,          limits.maxDescriptorSetSampledImages},
				{VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,          limits.maxDescriptorSetStorageImages},
				{VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER,   limits.maxDescriptorSetUniformBuffers},
				{VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER,   limits.maxDescriptorSetStorageBuffers},
				{VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,         limits.maxDescriptorSetUniformBuffers},
				{VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,         limits.maxDescriptorSetStorageBuffers},
				{VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, limits.maxDescriptorSetUniformBuffersDynamic},
				{VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, limits.maxDescriptorSetStorageBuffersDynamic},
				{VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT,       limits.maxDescriptorSetInputAttachments}
			};
			descInfo.poolSizeCount = SCast(Uint32)(poolSize.size());
			descInfo.pPoolSizes = poolSize.data();

			mDescriptorPool = device->CreateVkDescriptorPoolWrapper(descInfo, "DescriptorPool in VulkanShaderParameterManager");
		}

		VulkanShaderParameterManager::~VulkanShaderParameterManager()
		{
			mDescriptorPool.Release();
		}

		VkBuffer VulkanShaderParameterManager::GetUniformDynamicBuffer()
		{
			return mUniformHeap.GetHeapBuffer();
		}

		VkBuffer VulkanShaderParameterManager::GetStorageDynamicBuffer()
		{
			return mStorageHeap.GetHeapBuffer();
		}

		VulkanShaderParameterAllocation VulkanShaderParameterManager::Allocate(ShaderParameterType type, Uint64 size)
		{
			switch(type) {
			case ShaderParameterType::ConstantBuffer:
				return mUniformHeap.Allocate(size);

			case ShaderParameterType::StorageBuffer:
				return mStorageHeap.Allocate(size);

			case ShaderParameterType::RawData:
				// return AllocateRawData(size);
				ASSERTION_FAILED("Rawdata type is not implemented.");
				break;

			case ShaderParameterType::ConstantImage:
			case ShaderParameterType::StorageImage:
			case ShaderParameterType::SampledImage:
			case ShaderParameterType::Sampler:
				ASSERTION_FAILED("Image and sampler cannot be allocated to VulkanShaderParameterManager.");
				break;

			default:
				ASSERTION_FAILED("Unknown ShaderParameterType ({0})", (Uint32)type);
			}

			return {};
		}

		VulkanShaderParameterAllocation VulkanShaderParameterManager::AllocatePerFrame(ShaderParameterType type, Uint64 size)
		{
			switch(type) {
			case ShaderParameterType::ConstantBuffer:
				return mUniformHeap.AllocatePerFrame(size);

			case ShaderParameterType::StorageBuffer:
				return mStorageHeap.AllocatePerFrame(size);

			case ShaderParameterType::RawData:
				// return AllocateRawData(size);
				ASSERTION_FAILED("Rawdata type is not implemented.");
				break;

			case ShaderParameterType::ConstantImage:
			case ShaderParameterType::StorageImage:
			case ShaderParameterType::SampledImage:
			case ShaderParameterType::Sampler:
				ASSERTION_FAILED("Image and sampler cannot be allocated to VulkanShaderParameterManager.");
				break;

			default:
				ASSERTION_FAILED("Unknown ShaderParameterType ({0})", (Uint32)type);
			}

			return {};
		}

		void VulkanShaderParameterManager::Free(VulkanShaderParameterAllocation&& allocation)
		{
			// PerFrame allocation will be freed when calling DiscardFrame function
			if(allocation.isPerFrame)
				return;

			switch(allocation.type) {
			case ShaderParameterType::ConstantBuffer:
				mUniformHeap.Free(std::move(allocation));
				break;

			case ShaderParameterType::StorageBuffer:
				mStorageHeap.Free(std::move(allocation));
				break;

			case ShaderParameterType::RawData:
				// RawData allocation will be freed when calling DiscardFrame function
				break;

			case ShaderParameterType::ConstantImage:
			case ShaderParameterType::StorageImage:
			case ShaderParameterType::SampledImage:
			case ShaderParameterType::Sampler:
				ASSERTION_FAILED("Image and sampler cannot be freed to VulkanShaderParameterManager.");
				break;

			default:
				ASSERTION_FAILED("Unknown ShaderParameterType ({0})", (Uint32)allocation.type);
			}
		}

		void VulkanShaderParameterManager::DiscardPerFrame()
		{
			mUniformHeap.DiscardPerFrame();
			mStorageHeap.DiscardPerFrame();
		}

		VkDescriptorSet VulkanShaderParameterManager::AllocateDescriptorSet(VkDescriptorSetLayout layout, const char* layoutDebugName)
		{
			VkResult res;

			VkDescriptorSetAllocateInfo info;
			info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			info.pNext = nullptr;
			info.descriptorPool = mDescriptorPool.mObject;
			info.descriptorSetCount = 1;
			info.pSetLayouts = &layout;

			VkDescriptorSet set;
			res = vkAllocateDescriptorSets(mDescriptorPool.GetVkDevice(), &info, &set);
			CHECK_VK(res, "Failed to allocate descriptor set.");

			VulkanDebug::SetObjectName(mDescriptorPool.GetVkDevice(), set,
				fmt::format("DescriptorSet in {0}", layoutDebugName).c_str());

			return set;
		}

		void VulkanShaderParameterManager::FreeDescriptorSet(VkDescriptorSet descSet)
		{
			VkResult res;
			
			res = vkFreeDescriptorSets(mDescriptorPool.GetVkDevice(), mDescriptorPool.mObject, 1, &descSet);
			CHECK_VK(res, "Failed to free descriptor set.");
		}
	} // namespace render
} // namespace cube
