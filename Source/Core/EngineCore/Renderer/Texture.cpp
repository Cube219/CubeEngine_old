#include "Texture.h"

#include "RendererManager.h"
#include "BaseRenderAPI/Wrapper/BaseRenderFence.h"

namespace cube
{
	namespace core
	{
		Texture::Texture(UPtr<RendererManager>& manager, char* data, uint64_t size, uint32_t width, uint32_t height) :
			mManager_ref(manager), mData(data), mSize(size), mWidth(width), mHeight(height)
		{
			auto renderAPI = manager->GetRenderAPI();

			// Add image data in buffer
			BaseRenderBufferInitializer bufInit;
			bufInit.type = BufferTypeBits::TransferSource;

			BaseRenderBufferInitializer::BufferData bufData;
			bufData.data = nullptr;
			bufData.size = size;
			bufInit.bufferDatas.push_back(bufData);

			mStagingBuffer = renderAPI->CreateBuffer(bufInit);

			mStagingBuffer->Map();
			mStagingBuffer->UpdateBufferData(0, data, size);
			mStagingBuffer->Unmap();

			BaseRenderImageInitializer init;
			init.type = ImageType::Image2D;
			init.format = DataFormat::R8G8B8A8_Unorm;
			init.width = width;
			init.height = height;
			init.depth = 1;
			init.mipLevels = 1;
			init.usage = ImageUsageBits::TransferDestinationBit | ImageUsageBits::SampledBit;
			mImage = renderAPI->CreateImage(init);

			// Copy buffer to image
			auto cmd = renderAPI->CreateCommandBuffer();
			auto cmdSubmitFence = renderAPI->CreateFence();
			cmd->Begin();

			cmd->PipelineImageMemoryBarrier(PipelineStageBits::TopBit, PipelineStageBits::TransferBit, AccessBits::None, AccessBits::TransferWriteBit,
				ImageLayout::Undefined, ImageLayout::TransferDestinationOptimal, mImage); // Undefined -> transfer
			cmd->CopyBufferToImage(mStagingBuffer, 0, mImage, 0, 0, 0, width, height, 1, ImageAspectBits::Color);
			cmd->PipelineImageMemoryBarrier(PipelineStageBits::TransferBit, PipelineStageBits::FragmentShaderBit, AccessBits::TransferWriteBit, AccessBits::ShaderReadBit,
				ImageLayout::TransferDestinationOptimal, ImageLayout::ShaderReadOnlyOptimal, mImage); // Transfer -> fragment shader reading

			cmd->End();

			auto graphicsQueue = renderAPI->GetQueue(QueueTypeBits::GraphicsBit, 0);
			cmd->Submit(graphicsQueue, 0, nullptr, 0, nullptr, cmdSubmitFence);
			cmdSubmitFence->Wait(100000000);

			mImageView = mImage->GetImageView(DataFormat::R8G8B8A8_Unorm, ImageAspectBits::Color, ImageViewType::Image2D);

			mSampler = renderAPI->CreateSampler();
		}

		Texture::~Texture()
		{
		}

		void Texture::SendTextureData(SPtr<BaseRenderCommandBuffer>& commandBuffer)
		{
			commandBuffer->CopyBufferToImage(mStagingBuffer, 0, mImage, 0, 0, 0, mWidth, mHeight, 1, ImageAspectBits::Color);
		}
	}
}