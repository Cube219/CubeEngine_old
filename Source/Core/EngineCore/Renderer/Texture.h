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
		class ENGINE_CORE_EXPORT TextureImporter : public ResourceImporter
		{
		public:
			TextureImporter(SPtr<render::RenderAPI>& renderAPI) : 
				mRenderAPI(renderAPI)
			{
				mResName = CUBE_T("texture");
			}

			Resource* Import(SPtr<platform::File>& file, Json info) final override;

		private:
			SPtr<render::RenderAPI> mRenderAPI;
		};

		class ENGINE_CORE_EXPORT Texture : public Resource
		{
		public:
			static RPtr<Texture> Load(const String& path);

		public:
			virtual ~Texture();

			void SendTextureData(SPtr<render::CommandBuffer>& commandBuffer);

			SPtr<render::ImageView> GetImageView() const { return mImageView; }
			SPtr<render::Sampler> GetSampler() const { return mSampler; }

		private:
			friend class TextureImporter;
			Texture(){ }

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
