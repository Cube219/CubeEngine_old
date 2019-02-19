#pragma once

#include "../VulkanAPIHeader.h"

namespace cube
{
	namespace render
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

			VkPhysicalDevice GetHandle() const { return mPhysicalDevice; }

		private:
			VulkanPhysicalDevice(VkPhysicalDevice physicalDevice);
			VkPhysicalDevice mPhysicalDevice;
		};
	} // namespace render
} // namespace cube
