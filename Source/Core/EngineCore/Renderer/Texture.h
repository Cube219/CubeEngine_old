#pragma once

#include "../EngineCoreHeader.h"

#include "../Resource/BaseResource.h"
#include "BaseRenderAPI/Wrapper/BaseRenderBuffer.h"
#include "BaseRenderAPI/Wrapper/BaseRenderImage.h"
#include "BaseRenderAPI/Wrapper/BaseRenderSampler.h"
#include "BaseRenderAPI/Wrapper/BaseRenderCommandBuffer.h"

namespace cube
{
	namespace core
	{
		class ENGINE_CORE_EXPORT Texture : public BaseResource
		{
		public:
			~Texture();

			void SendTextureData(SPtr<BaseRenderCommandBuffer>& commandBuffer);

			SPtr<BaseRenderImageView> GetImageView() const { return mImageView; }
			SPtr<BaseRenderSampler> GetSampler() const { return mSampler; }

		private:
			friend class BaseResource;
			Texture(ResourceRawData* rawData);

			uint64_t mImageSize;

			uint32_t mWidth;
			uint32_t mHeight;

			uint32_t mMipLevel;

			SPtr<BaseRenderBuffer> mStagingBuffer;
			SPtr<BaseRenderImage> mImage;
			SPtr<BaseRenderImageView> mImageView;
			SPtr<BaseRenderSampler> mSampler;

			SPtr<RendererManager> mManager_ref;
		};
	}
}