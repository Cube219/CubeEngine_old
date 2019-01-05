#pragma once

#include "../EngineCoreHeader.h"

#include "../Resource/BaseResource.h"
#include "BaseRenderAPI/Wrapper/Buffer.h"
#include "BaseRenderAPI/Wrapper/Image.h"
#include "BaseRenderAPI/Wrapper/Sampler.h"
#include "BaseRenderAPI/Wrapper/CommandBuffer.h"
#include "BaseRenderAPI/RenderAPI.h"

namespace cube
{
	namespace core
	{
		class ENGINE_CORE_EXPORT Texture : public Resource
		{
		public:
			static RPtr<Texture> Load(StringRef path);

		public:
			Texture(){ }
			virtual ~Texture() { }

			void WriteData(SPtr<render::RenderAPI>& renderAPI, void* data, uint64_t size, uint32_t width, uint32_t height);

			void SendTextureData(SPtr<render::CommandBuffer>& commandBuffer);

			SPtr<render::ImageView> GetImageView() const { return mImageView; }
			SPtr<render::Sampler> GetSampler() const { return mSampler; }

		private:
			uint64_t mImageSize;

			uint32_t mWidth;
			uint32_t mHeight;

			uint32_t mMipLevel;

			SPtr<render::Buffer> mStagingBuffer;
			SPtr<render::Image> mImage;
			SPtr<render::ImageView> mImageView;
			SPtr<render::Sampler> mSampler;

			SPtr<RendererManager> mManager_ref;
		};
	} // namespace core
} // namespace cube
