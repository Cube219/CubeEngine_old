#pragma once

#include "../EngineCoreHeader.h"

#include "BaseRenderAPI/Wrapper/BaseRenderBuffer.h"
#include "BaseRenderAPI/Wrapper/BaseRenderImage.h"
#include "BaseRenderAPI/Wrapper/BaseRenderSampler.h"
#include "BaseRenderAPI/Wrapper/BaseRenderCommandBuffer.h"

namespace cube
{
	namespace core
	{
		class ENGINE_CORE_EXPORT Texture
		{
		public:
			Texture(UPtr<RendererManager>& manager, char* data, uint64_t size, uint32_t width, uint32_t height);
			~Texture();

			void SendTextureData(SPtr<BaseRenderCommandBuffer>& commandBuffer);

		private:

			char* mData;
			uint64_t mSize;

			uint32_t mWidth;
			uint32_t mHeight;

			uint32_t mMipLevel;

			SPtr<BaseRenderBuffer> mStagingBuffer;
			SPtr<BaseRenderImage> mImage;
			SPtr<BaseRenderImageView> mImageView;
			SPtr<BaseRenderSampler> mSampler;

			UPtr<RendererManager>& mManager_ref;
		};
	}
}