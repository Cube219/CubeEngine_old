#pragma once

#include "../EngineCoreHeader.h"

#include "../Resource/BaseResource.h"
#include "RenderAPI/Interface/Texture.h"
#include "RenderAPI/Interface/TextureView.h"
#include "RenderAPI/Interface/Sampler.h"
#include "RenderAPI/Interface/Device.h"

namespace cube
{
	class ENGINE_CORE_EXPORT Texture : public Resource
	{
	public:
		static RPtr<Texture> Load(StringRef path);

	public:
		Texture(){ }
		virtual ~Texture() { }

		void WriteData(SPtr<render::Device>& device, void* data, Uint64 size, Uint32 width, Uint32 height);

		SPtr<render::TextureView> GetTextureView() const { return mTextureView; }
		SPtr<render::Sampler> GetSampler() const { return mSampler; }

	private:
		Uint64 mImageSize;

		Uint32 mWidth;
		Uint32 mHeight;

		Uint32 mMipLevel;

		SPtr<render::Texture> mTexture;
		SPtr<render::TextureView> mTextureView;
		SPtr<render::Sampler> mSampler;

		SPtr<RendererManager> mManager_ref;
	};
} // namespace cube
