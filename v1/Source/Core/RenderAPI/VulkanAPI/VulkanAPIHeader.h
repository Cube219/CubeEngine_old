#pragma once
#pragma warning(disable:4251)
#pragma warning(disable:4275)

#include "Base/BaseTypes.h"
#include "RenderAPI/RenderAPIHeader.h"

#include <vulkan/vulkan.h>

namespace cube
{
	namespace render
	{
		class DeviceVk;

		class BufferVk;
		class BufferViewVk;

		class CommandListVk;
		class FenceVk;

		class GraphicsPipelineStateVk;
		class ComputePipelineStateVk;

		class TextureVk;
		class TextureViewVk;

		class SwapChainVk;

		class ShaderVk;
		class ShaderParametersVk;
		class ShaderParametersLayoutVk;

		class SamplerVk;

		class RenderTargetVk;
		class RenderPassVk;

		class VulkanPhysicalDevice;
		class VulkanLogicalDevice;

		struct VulkanAllocation;
		class VulkanMemoryPage;
		class VulkanMemoryHeap;
		class VulkanMemoryManager;

		struct VulkanUploadAllocation;
		class VulkanUploadHeap;

		struct VulkanShaderParameterAllocation;
		class VulkanShaderParameterHeap;
		class VulkanShaderParameterManager;

		class VulkanQueueManager;

		class VulkanCommandListPool;
		class VulkanFencePool;
		class VulkanSemaphorePool;
	} // namespace render
} // namespace cube

#ifdef VULKAN_API_EXPORTS
#define VULKAN_API_EXPORT __declspec(dllexport) 
#else // VULKAN_API_EXPORTS
#define VULKAN_API_EXPORT __declspec(dllimport) 
#endif // VULKAN_API_EXPORTS
