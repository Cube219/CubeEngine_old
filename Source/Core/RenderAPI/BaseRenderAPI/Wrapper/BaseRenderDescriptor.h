#pragma once

#include "../BaseRenderAPIHeader.h"

namespace cube
{
	namespace core
	{
		enum class DescriptorType
		{
			Sampler,
			CombinedImageSampler,
			SampledImage,
			StorageImage,
			UniformTexelBuffer,
			StorageTexelBuffer,
			UniformBuffer,
			StorageBuffer,
			UniformBufferDynamic,
			StorageBufferDynamic,
			InputAttachment
		};

		struct BaseRenderDescriptorSetInitializer
		{
			struct Descriptor
			{
				ShaderTypeBits shaderType;
				DescriptorType type;
				uint32_t bindingIndex;
				uint32_t count;
			};

			Vector<Descriptor> descriptors;
		};

		class BaseRenderDescriptorSetLayout
		{
		public:
			virtual ~BaseRenderDescriptorSetLayout(){ }

		protected:
			BaseRenderDescriptorSetLayout(){ }
		};

		class BaseRenderDescriptorSet
		{
		public:
			virtual ~BaseRenderDescriptorSet(){ }

			virtual void WriteBufferInDescriptor(uint32_t bindingIndex, uint32_t bufferNum, BaseRenderBufferInfo* buffers) = 0;
			virtual void WriteImagesInDescriptor(uint32_t bindingIndex, uint32_t imageNum, SPtr<BaseRenderImageView>* imageViews, SPtr<BaseRenderSampler>* samplers) = 0;

		protected:
			BaseRenderDescriptorSet(){ }
		};
	}
}