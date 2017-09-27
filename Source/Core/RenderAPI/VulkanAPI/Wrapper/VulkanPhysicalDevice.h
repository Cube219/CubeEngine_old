#pragma once

#include "..\VulkanAPIHeader.h"

namespace cube
{
	namespace core
	{
		enum class VulkanPhysicalDeviceFeature
		{
			RobustBufferAccess,
			FullDrawIndexUint32,
			ImageCubeArray,
			IndependentBlend,
			GeometryShader,
			TessellationShader,
			SampleRateShading,
			DualScrBlend,
			LogicOp,
			MultiDrawIndirect,
			DrawIndirectFirstInstance,
			DepthClamp,
			DepthBiasClamp,
			FillModeNonSolid,
			DepthBounds,
			WideLines,
			LargePoints,
			AlphaToOne,
			MultiViewport,
			SamplerAnisotropy,
			TextureCompressionETC2,
			TextureCompressionASTC_LDR,
			TextureCompressionBC,
			OcclusionQueryPrecise,
			PipelineStatisticQuery,
			VertexPipelineStoresAndAtomics,
			FragmentStoresAndAtomics,
			ShaderTessellationAndGeometryPointSize,
			ShaderImageGatherExtended,
			ShaderStorageImageExtendedFormats,
			ShaderStorageImageMultisample,
			ShaderStorageImageReadWithoutFormat,
			ShaderStorageImageWriteWithoutFormat,
			ShaderUniformBufferArrayDynamicIndexing,
			ShaderSampledImageArrayDynamicIndexing,
			ShaderStorageBufferArrayDynamicIndexing,
			ShaderStorageImageArrayDynamicIndexing,
			ShaderClipDistance,
			ShaderCullDistance,
			ShaderFloat64,
			ShaderInt64,
			ShaderInt16,
			ShaderResourceResidency,
			ShaderResourceMinLod,
			SparseBinding,
			SparseResidencyBuffer,
			SparseResidencyImage2D,
			SparseResidencyImage3D,
			SparseResidency2Samples,
			SparseResidency4Samples,
			SparseResidency8Samples,
			SparseResidency16Samples,
			SparseResidencyAliased,
			VariableMultisampleRate,
			InheritedQueries
		};
		class VulkanPhysicalDeviceFeatures; // Declared at VulkanAPIHeader.h
		// Because it is used like a structure, not a class using a pointer

		class VulkanPhysicalDevice
		{
			friend VulkanInstance;

		public:
			~VulkanPhysicalDevice();

			operator VkPhysicalDevice() const
			{
				return mPhysicalDevice;
			}

			const VkPhysicalDeviceProperties GetProperties() const;
			const VkPhysicalDeviceFeatures GetFeatures() const;
			const VkPhysicalDeviceMemoryProperties GetMemProperties() const;

			const Vector<VulkanQueueFamily>& GetAllQueueFamily();
			VulkanQueueFamily GetQueueFamily(VkQueueFlags type);

		private:
			VulkanPhysicalDevice(){ }
			VulkanPhysicalDevice(VkPhysicalDevice physicalDevice);
			VkPhysicalDevice mPhysicalDevice;

			VkPhysicalDeviceProperties mProperties;
			VkPhysicalDeviceFeatures mFeatures;
			VkPhysicalDeviceMemoryProperties mMemProperties;

			Vector<VulkanQueueFamily> mQueueFamilies;
		};

		inline const Vector<VulkanQueueFamily>& VulkanPhysicalDevice::GetAllQueueFamily()
		{
			return mQueueFamilies;
		}

		inline const VkPhysicalDeviceProperties VulkanPhysicalDevice::GetProperties() const
		{
			return mProperties;
		}

		inline const VkPhysicalDeviceFeatures VulkanPhysicalDevice::GetFeatures() const
		{
			return mFeatures;
		}

		inline const VkPhysicalDeviceMemoryProperties VulkanPhysicalDevice::GetMemProperties() const
		{
			return mMemProperties;
		}
	}
}