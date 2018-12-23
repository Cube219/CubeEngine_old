#pragma once

#include "VulkanAPIHeader.h"

#include "VkObject.h"
#include "VulkanMemoryManager.h"

namespace cube
{
	namespace render
	{
		struct VulkanShaderParameterAllocation
		{
			ShaderParameterType type;
			bool isPerFrame;
			Uint32 bindIndex;

			VkBuffer buffer;
			Uint64 dynamicOffset;

			Uint64 size;
			void* pData;
		};

		///////////////////////////////
		// VulkanShaderParameterHeap //
		///////////////////////////////

		constexpr Uint32 UniformBufferMaxSize = 4 * 1024 * 1024;  // 4MiB
		constexpr Uint32 StorageBufferMaxSize = 64 * 1024 * 1024; // 64MiB

		class VulkanShaderParameterHeap
		{
		public:
			VulkanShaderParameterHeap(SPtr<VulkanLogicalDevice>& device, VulkanMemoryManager& memManager,
				Uint32 heapIndex, ShaderParameterType type);
			~VulkanShaderParameterHeap();

			VkBuffer GetHeapBuffer() const { return mBuffer.mObject; }

			// TODO: alignment 구현
			VulkanShaderParameterAllocation Allocate(Uint64 size);
			VulkanShaderParameterAllocation AllocatePerFrame(Uint64 size);
			void Free(VulkanShaderParameterAllocation&& allocation);

			void DiscardPerFrame();

		private:
			Uint32 mHeapIndex;
			ShaderParameterType mHeapType;

			VkBufferWrapper mBuffer;
			VulkanAllocation mBufferMemoryAllocation;
			Uint64 mBufferSize;
			Uint64 mAlignment;

			Uint64 mCurrentOffset;

			Uint64 mFrameOffset;
			Uint64 mCurrentFrameOffset;
		};

		//////////////////////////////////
		// VulkanShaderParameterManager //
		//////////////////////////////////

		constexpr Uint32 RawDataBufferSize = 256;

		class VulkanShaderParameterManager
		{
		public:
			VulkanShaderParameterManager(SPtr<VulkanLogicalDevice>& device, VulkanMemoryManager& memManager);
			~VulkanShaderParameterManager();

			VkBuffer GetUniformDynamicBuffer();
			VkBuffer GetStorageDynamicBuffer();

			VulkanShaderParameterAllocation Allocate(ShaderParameterType type, Uint64 size);
			VulkanShaderParameterAllocation AllocatePerFrame(ShaderParameterType type, Uint64 size);
			void Free(VulkanShaderParameterAllocation&& allocation);

			void DiscardPerFrame();

			VkDescriptorSet AllocateDescriptorSet(VkDescriptorSetLayout layout, const char* layoutDebugName);
			void FreeDescriptorSet(VkDescriptorSet descSet);

		private:
			VulkanShaderParameterAllocation AllocateRawData(Uint64 size);

			VulkanShaderParameterHeap mUniformHeap;
			VulkanShaderParameterHeap mStorageHeap;

			VkDescriptorPoolWrapper mDescriptorPool;

			bool mIsRawDataAllocated = false;
			char mRawDataBuffer[RawDataBufferSize];
		};
	} // namespace render
} // namespace cube
