#include "VulkanPhysicalDevice.h"

#include "EngineCore/Assertion.h"
#include "VulkanQueue.h"

namespace cube
{
	namespace render
	{
		VulkanPhysicalDeviceFeatures::VulkanPhysicalDeviceFeatures()
		{
			int vkFeatureSize = sizeof(VkPhysicalDeviceFeatures);
			int vkBool32Size = sizeof(VkBool32);

			for(int i = 0; i < vkFeatureSize / vkBool32Size; i++) {
				VkBool32* t1 = (reinterpret_cast<VkBool32*>(&this->vkFeatures) + i);
				*t1 = 0;
			}
		}

		VulkanPhysicalDeviceFeatures::VulkanPhysicalDeviceFeatures(VkPhysicalDeviceFeatures features)
		{
			vkFeatures = features;
		}

		void VulkanPhysicalDeviceFeatures::SetFeature(VulkanPhysicalDeviceFeature feature, VkBool32 on)
		{
			switch(feature) {
				case VulkanPhysicalDeviceFeature::RobustBufferAccess:
					vkFeatures.robustBufferAccess = on;
					break;
				case VulkanPhysicalDeviceFeature::FullDrawIndexUint32:
					vkFeatures.fullDrawIndexUint32 = on;
					break;
				case VulkanPhysicalDeviceFeature::ImageCubeArray:
					vkFeatures.imageCubeArray = on;
					break;
				case VulkanPhysicalDeviceFeature::IndependentBlend:
					vkFeatures.independentBlend = on;
					break;
				case VulkanPhysicalDeviceFeature::GeometryShader:
					vkFeatures.geometryShader = on;
					break;
				case VulkanPhysicalDeviceFeature::TessellationShader:
					vkFeatures.tessellationShader = on;
					break;
				case VulkanPhysicalDeviceFeature::SampleRateShading:
					vkFeatures.sampleRateShading = on;
					break;
				case VulkanPhysicalDeviceFeature::DualScrBlend:
					vkFeatures.dualSrcBlend = on;
					break;
				case VulkanPhysicalDeviceFeature::LogicOp:
					vkFeatures.logicOp = on;
					break;
				case VulkanPhysicalDeviceFeature::MultiDrawIndirect:
					vkFeatures.multiDrawIndirect = on;
					break;
				case VulkanPhysicalDeviceFeature::DrawIndirectFirstInstance:
					vkFeatures.drawIndirectFirstInstance = on;
					break;
				case VulkanPhysicalDeviceFeature::DepthClamp:
					vkFeatures.depthClamp = on;
					break;
				case VulkanPhysicalDeviceFeature::DepthBiasClamp:
					vkFeatures.depthBiasClamp = on;
					break;
				case VulkanPhysicalDeviceFeature::FillModeNonSolid:
					vkFeatures.fillModeNonSolid = on;
					break;
				case VulkanPhysicalDeviceFeature::DepthBounds:
					vkFeatures.depthBounds = on;
					break;
				case VulkanPhysicalDeviceFeature::WideLines:
					vkFeatures.wideLines = on;
					break;
				case VulkanPhysicalDeviceFeature::LargePoints:
					vkFeatures.largePoints = on;
					break;
				case VulkanPhysicalDeviceFeature::AlphaToOne:
					vkFeatures.alphaToOne = on;
					break;
				case VulkanPhysicalDeviceFeature::MultiViewport:
					vkFeatures.multiViewport = on;
					break;
				case VulkanPhysicalDeviceFeature::SamplerAnisotropy:
					vkFeatures.samplerAnisotropy = on;
					break;
				case VulkanPhysicalDeviceFeature::TextureCompressionETC2:
					vkFeatures.textureCompressionETC2 = on;
					break;
				case VulkanPhysicalDeviceFeature::TextureCompressionASTC_LDR:
					vkFeatures.textureCompressionASTC_LDR = on;
					break;
				case VulkanPhysicalDeviceFeature::TextureCompressionBC:
					vkFeatures.textureCompressionBC = on;
					break;
				case VulkanPhysicalDeviceFeature::OcclusionQueryPrecise:
					vkFeatures.occlusionQueryPrecise = on;
					break;
				case VulkanPhysicalDeviceFeature::PipelineStatisticQuery:
					vkFeatures.pipelineStatisticsQuery = on;
					break;
				case VulkanPhysicalDeviceFeature::VertexPipelineStoresAndAtomics:
					vkFeatures.vertexPipelineStoresAndAtomics = on;
					break;
				case VulkanPhysicalDeviceFeature::FragmentStoresAndAtomics:
					vkFeatures.fragmentStoresAndAtomics = on;
					break;
				case VulkanPhysicalDeviceFeature::ShaderTessellationAndGeometryPointSize:
					vkFeatures.shaderTessellationAndGeometryPointSize = on;
					break;
				case VulkanPhysicalDeviceFeature::ShaderImageGatherExtended:
					vkFeatures.shaderImageGatherExtended = on;
					break;
				case VulkanPhysicalDeviceFeature::ShaderStorageImageExtendedFormats:
					vkFeatures.shaderStorageImageExtendedFormats = on;
					break;
				case VulkanPhysicalDeviceFeature::ShaderStorageImageMultisample:
					vkFeatures.shaderStorageImageMultisample = on;
					break;
				case VulkanPhysicalDeviceFeature::ShaderStorageImageReadWithoutFormat:
					vkFeatures.shaderStorageImageReadWithoutFormat = on;
					break;
				case VulkanPhysicalDeviceFeature::ShaderStorageImageWriteWithoutFormat:
					vkFeatures.shaderStorageImageWriteWithoutFormat = on;
					break;
				case VulkanPhysicalDeviceFeature::ShaderUniformBufferArrayDynamicIndexing:
					vkFeatures.shaderUniformBufferArrayDynamicIndexing = on;
					break;
				case VulkanPhysicalDeviceFeature::ShaderSampledImageArrayDynamicIndexing:
					vkFeatures.shaderSampledImageArrayDynamicIndexing = on;
					break;
				case VulkanPhysicalDeviceFeature::ShaderStorageBufferArrayDynamicIndexing:
					vkFeatures.shaderStorageBufferArrayDynamicIndexing = on;
					break;
				case VulkanPhysicalDeviceFeature::ShaderStorageImageArrayDynamicIndexing:
					vkFeatures.shaderStorageImageArrayDynamicIndexing = on;
					break;
				case VulkanPhysicalDeviceFeature::ShaderClipDistance:
					vkFeatures.shaderClipDistance = on;
					break;
				case VulkanPhysicalDeviceFeature::ShaderCullDistance:
					vkFeatures.shaderCullDistance = on;
					break;
				case VulkanPhysicalDeviceFeature::ShaderFloat64:
					vkFeatures.shaderFloat64 = on;
					break;
				case VulkanPhysicalDeviceFeature::ShaderInt64:
					vkFeatures.shaderInt64 = on;
					break;
				case VulkanPhysicalDeviceFeature::ShaderInt16:
					vkFeatures.shaderInt16 = on;
					break;
				case VulkanPhysicalDeviceFeature::ShaderResourceResidency:
					vkFeatures.shaderResourceResidency = on;
					break;
				case VulkanPhysicalDeviceFeature::ShaderResourceMinLod:
					vkFeatures.shaderResourceMinLod = on;
					break;
				case VulkanPhysicalDeviceFeature::SparseBinding:
					vkFeatures.sparseBinding = on;
					break;
				case VulkanPhysicalDeviceFeature::SparseResidencyBuffer:
					vkFeatures.sparseResidencyBuffer = on;
					break;
				case VulkanPhysicalDeviceFeature::SparseResidencyImage2D:
					vkFeatures.sparseResidencyImage2D = on;
					break;
				case VulkanPhysicalDeviceFeature::SparseResidencyImage3D:
					vkFeatures.sparseResidencyImage3D = on;
					break;
				case VulkanPhysicalDeviceFeature::SparseResidency2Samples:
					vkFeatures.sparseResidency2Samples = on;
					break;
				case VulkanPhysicalDeviceFeature::SparseResidency4Samples:
					vkFeatures.sparseResidency4Samples = on;
					break;
				case VulkanPhysicalDeviceFeature::SparseResidency8Samples:
					vkFeatures.sparseResidency8Samples = on;
					break;
				case VulkanPhysicalDeviceFeature::SparseResidency16Samples:
					vkFeatures.sparseResidency16Samples = on;
					break;
				case VulkanPhysicalDeviceFeature::SparseResidencyAliased:
					vkFeatures.sparseResidencyAliased = on;
					break;
				case VulkanPhysicalDeviceFeature::VariableMultisampleRate:
					vkFeatures.variableMultisampleRate = on;
					break;
				case VulkanPhysicalDeviceFeature::InheritedQueries:
					vkFeatures.inheritedQueries = on;
					break;

				default:
					CUBE_LOG(cube::LogType::Warning, "Undefined Feature ({0})", (int)feature);
					break;
			}
		}

		VulkanPhysicalDeviceFeatures VulkanPhysicalDeviceFeatures::operator&(const VulkanPhysicalDeviceFeatures& other)
		{
			VulkanPhysicalDeviceFeatures res;

			int vkFeatureSize = sizeof(VkPhysicalDeviceFeatures);
			int vkBool32Size = sizeof(VkBool32);

			for(int i = 0; i < vkFeatureSize / vkBool32Size; i++) {
				const VkBool32* t1 = (reinterpret_cast<const VkBool32*>(&this->vkFeatures) + i);
				const VkBool32* t2 = (reinterpret_cast<const VkBool32*>(&other.vkFeatures) + i);
				*(reinterpret_cast<VkBool32*>(&res) + i) = (*t1) & (*t2);
			}

			return res;
		}

		VulkanPhysicalDeviceFeatures VulkanPhysicalDeviceFeatures::operator|(const VulkanPhysicalDeviceFeatures& other)
		{
			VulkanPhysicalDeviceFeatures res;

			int vkFeatureSize = sizeof(VkPhysicalDeviceFeatures);
			int vkBool32Size = sizeof(VkBool32);

			for(int i = 0; i < vkFeatureSize / vkBool32Size; i++) {
				const VkBool32* t1 = (reinterpret_cast<const VkBool32*>(&this->vkFeatures) + i);
				const VkBool32* t2 = (reinterpret_cast<const VkBool32*>(&other.vkFeatures) + i);
				*(reinterpret_cast<VkBool32*>(&res) + i) = (*t1) | (*t2);
			}

			return res;
		}

		// ------------------------------------------------------------
		//                     VulkanPhysicalDevice
		// ------------------------------------------------------------

		VulkanPhysicalDevice::VulkanPhysicalDevice(VkPhysicalDevice physicalDevice) :
			mPhysicalDevice(physicalDevice)
		{
		}

		VulkanPhysicalDevice::~VulkanPhysicalDevice()
		{
		}
	} // namespace render
} // namespace cube
