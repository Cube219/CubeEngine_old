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
			mStagingBuffer = renderAPI->CreateBuffer(size, BufferTypeBits::TransferSource);

			mStagingBuffer->Map();
			mStagingBuffer->UpdateBufferData(data, size, 0);
			mStagingBuffer->Unmap();

			mImage = renderAPI->CreateImage(ImageType::Image2D, DataFormat::R8G8B8A8_Unorm,
				width, height, 1, 1,
				ImageUsageBits::TransferDestinationBit | ImageUsageBits::SampledBit);

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