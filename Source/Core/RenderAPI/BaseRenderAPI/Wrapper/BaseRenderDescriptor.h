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

		class BaseRenderDescriptorSet
		{
		public:
			virtual ~BaseRenderDescriptorSet(){ }

			virtual void AddDescriptor(ShaderType shaderType, DescriptorType descriptorType, uint32_t bindingIndex, uint32_t count) = 0;

			virtual void Create() = 0;

			virtual void WriteBufferInDescriptor(uint32_t bindingIndex, uint32_t bufferNum, BaseRenderBufferInfo* buffers) = 0;
			virtual void WriteImagesInDescriptor(uint32_t bindingIndex, uint32_t imageNum, SPtr<BaseRenderImageView>* imageViews, SPtr<BaseRenderSampler>* samplers) = 0;

			/*	virtual void AddDescriptor(ShaderType shaderType, uint32_t bindingIndex,
				uint32_t imageNum, std::shared_ptr<BaseRenderBuffer>* buffers) = 0;

			virtual void AddDescriptor(ShaderType shaderType, uint32_t bindingIndex,
				uint32_t texelNum, std::shared_ptr<BaseRenderBuffer>* buffers) = 0;*/
				/*
				virtual void WriteBuffer(uint32_t bindingIndex, uint32_t bufferNum, BaseRenderBuffer::Info* buffers);
				virtual void WriteImage(uint32_t bindingIndex, uint32_t imageNum);
				virtual void WriteTexel(uint32_t bindingIndex, uint32_t texelNum);
				*/

		protected:
			BaseRenderDescriptorSet(){ }
		};
	}
}