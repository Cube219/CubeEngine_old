#pragma once

#include "../EngineCoreHeader.h"

#include "../Resource/BaseResource.h"
#include "RenderObject.h"
#include "RenderAPI/Interface/Texture.h"
#include "RenderAPI/Interface/TextureView.h"
#include "RenderAPI/Interface/Sampler.h"
#include "RenderAPI/Interface/Device.h"

namespace cube
{
	class ENGINE_CORE_EXPORT Texture : public Resource, public RenderObject
	{
	public:
		static RPtr<Texture> Load(StringRef path);

	public:
		Texture();
		Texture(const SPtr<TextureData>& initialData);
		virtual ~Texture();

		virtual SPtr<rt::RenderObject> CreateRenderObject() const override;
		SPtr<rt::Texture> GetRenderObject() const { return DPCast(rt::Texture)(mRenderObject); }

	private:
		mutable SPtr<TextureData> mTextureData;
		Uint32 mMipLevel;
	};

	namespace rt
	{
		class Texture : public RenderObject
		{
		public:
			Texture(const SPtr<TextureData>& initialData);
			virtual ~Texture();
			virtual void Initialize() override;

			void SyncTextureData(const SPtr<TextureData>& textureData);

			SPtr<render::Texture> GetTexture() const { return mTexture; }
			SPtr<render::TextureView> GetTextureView() const { return mTextureView; }
			SPtr<render::Sampler> GetSampler() const { return mSampler; }

		private:
			friend class cube::Texture;

			void FlushToRenderTexture();

			SPtr<TextureData> mTextureData;

			SPtr<render::Texture> mTexture;
			SPtr<render::TextureView> mTextureView;
			SPtr<render::Sampler> mSampler;
		};
	} // namespace rt
} // namespace cube
