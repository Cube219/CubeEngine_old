#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "../EngineCore.h"
#include "../Resource/ResourceManager.h"
#include "RendererManager.h"
#include "BaseRenderAPI/Wrapper/Fence.h"


namespace cube
{
	namespace core
	{
		Resource* TextureImporter::Import(SPtr<platform::File>& file, Json info)
		{
			uint64_t size = file->GetFileSize();

			void* rawData = malloc(size);
			uint64_t readSize = 0;
			file->Read(rawData, size, readSize);

			int width, height, channel;
			stbi_uc* imageData = stbi_load_from_memory(SCast(const stbi_uc*)(rawData), (int)(size), &width, &height, &channel, STBI_rgb_alpha);
			
			// Create texture resource
			Texture* texture = new Texture();
			texture->mWidth = width;
			texture->mHeight = height;
			texture->mImageSize = width * height * 4;

			// Add image data in buffer
			render::BufferInitializer bufInit;
			bufInit.type = render::BufferTypeBits::TransferSource;

			render::BufferInitializer::BufferData bufData;
			bufData.data = nullptr;
			bufData.size = texture->mImageSize;
			bufInit.bufferDatas.push_back(bufData);

			texture->mStagingBuffer = mRenderAPI->CreateBuffer(bufInit);

			texture->mStagingBuffer->Map();
			texture->mStagingBuffer->UpdateBufferData(0, imageData, texture->mImageSize);
			texture->mStagingBuffer->Unmap();

			render::ImageInitializer init;
			init.type = render::ImageType::Image2D;
			init.format = render::DataFormat::R8G8B8A8_Unorm;
			init.width = width;
			init.height = height;
			init.depth = 1;
			init.mipLevels = 1;
			init.usage = render::ImageUsageBits::TransferDestinationBit | render::ImageUsageBits::SampledBit;
			texture->mImage = mRenderAPI->CreateImage(init);

			// Copy buffer to image
			auto cmd = mRenderAPI->CreateCommandBuffer();
			auto cmdSubmitFence = mRenderAPI->CreateFence();
			cmd->Begin();

			using namespace render;
			cmd->PipelineImageMemoryBarrier(render::PipelineStageBits::TopBit, PipelineStageBits::TransferBit, AccessBits::None, AccessBits::TransferWriteBit,
				ImageLayout::Undefined, ImageLayout::TransferDestinationOptimal, texture->mImage); // Undefined -> transfer
			cmd->CopyBufferToImage(texture->mStagingBuffer, 0, texture->mImage, 0, 0, 0, width, height, 1, ImageAspectBits::Color);
			cmd->PipelineImageMemoryBarrier(PipelineStageBits::TransferBit, PipelineStageBits::FragmentShaderBit, AccessBits::TransferWriteBit, AccessBits::ShaderReadBit,
				ImageLayout::TransferDestinationOptimal, ImageLayout::ShaderReadOnlyOptimal, texture->mImage); // Transfer -> fragment shader reading

			cmd->End();

			auto graphicsQueue = mRenderAPI->GetQueue(QueueTypeBits::GraphicsBit, 0);
			cmd->Submit(graphicsQueue, 0, nullptr, 0, nullptr, cmdSubmitFence);
			cmdSubmitFence->Wait(100000000);

			texture->mImageView = texture->mImage->GetImageView(DataFormat::R8G8B8A8_Unorm, ImageAspectBits::Color, ImageViewType::Image2D);

			texture->mSampler = mRenderAPI->CreateSampler();
			
			stbi_image_free(imageData);
			free(rawData);

			return texture;
		}

		Texture::~Texture()
		{
		}

		RPtr<Texture> Texture::Load(const String& path)
		{
			return ECore()->GetResourceManager()->LoadResource<Texture>(path);
		}

		void Texture::SendTextureData(SPtr<render::CommandBuffer>& commandBuffer)
		{
			commandBuffer->CopyBufferToImage(mStagingBuffer, 0, mImage, 0, 0, 0, mWidth, mHeight, 1, render::ImageAspectBits::Color);
		}
	} // namespace core
} // namespace cube
