#pragma once

#include "../RenderAPIHeader.h"

namespace cube
{
	namespace render
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

		struct DescriptorSetInitializer
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

		class DescriptorSetLayout
		{
		public:
			virtual ~DescriptorSetLayout(){ }

		protected:
			DescriptorSetLayout(){ }
		};

		class DescriptorSet
		{
		public:
			virtual ~DescriptorSet(){ }

			virtual void WriteBufferInDescriptor(uint32_t bindingIndex, uint32_t bufferNum, BufferInfo* buffers) = 0;
			virtual void WriteImagesInDescriptor(uint32_t bindingIndex, uint32_t imageNum, SPtr<ImageView>* imageViews, SPtr<Sampler>* samplers) = 0;

		protected:
			DescriptorSet(){ }
		};
	} // namespace render
} // namespace cube
