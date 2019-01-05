#include "Texture.h"

#include "../EngineCore.h"
#include "../Resource/ResourceManager.h"
#include "BaseRenderAPI/Wrapper/Fence.h"
#include "BaseRenderAPI/Wrapper/GraphicsPipeline.h"
#include "BaseRenderAPI/Wrapper/Queue.h"

namespace cube
{
	namespace core
	{
		RPtr<Texture> Texture::Load(StringRef path)
		{
			return ECore().GetResourceManager().LoadResource<Texture>(path);
		}

		void Texture::WriteData(SPtr<render::RenderAPI>& renderAPI, void* data, uint64_t size, uint32_t width, uint32_t height)
		{
			// Add image data in buffer
			render::BufferInitializer bufInit;
			bufInit.type = render::BufferTypeBits::TransferSource;

			render::BufferInitializer::BufferData bufData;
			bufData.data = nullptr;
			bufData.size = size;
			bufInit.bufferDatas.push_back(bufData);

			mStagingBuffer = renderAPI->CreateBuffer(bufInit);

			mStagingBuffer->Map();
			mStagingBuffer->UpdateBufferData(0, data, size);
			mStagingBuffer->Unmap();

			render::ImageInitializer init;
			init.type = render::ImageType::Image2D;
			init.format = render::DataFormat::R8G8B8A8_Unorm;
			init.width = width;
			init.height = height;
			init.depth = 1;
			init.mipLevels = 1;
			init.usage = render::ImageUsageBits::TransferDestinationBit | render::ImageUsageBits::SampledBit;
			mImage = renderAPI->CreateImage(init);

			// Copy buffer to image
			auto cmd = renderAPI->CreateCommandBuffer();
			auto cmdSubmitFence = renderAPI->CreateFence();
			cmd->Begin();

			using namespace render;
			cmd->PipelineImageMemoryBarrier(render::PipelineStageBits::TopBit, PipelineStageBits::TransferBit, AccessBits::None, AccessBits::TransferWriteBit,
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

		void Texture::SendTextureData(SPtr<render::CommandBuffer>& commandBuffer)
		{
			commandBuffer->CopyBufferToImage(mStagingBuffer, 0, mImage, 0, 0, 0, mWidth, mHeight, 1, render::ImageAspectBits::Color);
		}
	} // namespace core
} // namespace cube
