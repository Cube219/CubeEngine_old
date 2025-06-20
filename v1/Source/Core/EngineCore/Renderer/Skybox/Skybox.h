#pragma once

#include "../../EngineCoreHeader.h"

#include "../RenderObject.h"
#include "RenderAPI/Interface/Texture.h"
#include "RenderAPI/Interface/TextureView.h"
#include "RenderAPI/Interface/Sampler.h"

namespace cube
{
	struct SkyboxInitializer
	{
		RPtr<Texture> textureArray = nullptr;

		RPtr<Texture> textureRight = nullptr;
		RPtr<Texture> textureLeft = nullptr;
		RPtr<Texture> textureTop = nullptr;
		RPtr<Texture> textureBottom = nullptr;
		RPtr<Texture> textureBack = nullptr;
		RPtr<Texture> textureFront = nullptr;
	};

	class ENGINE_CORE_EXPORT Skybox : public RenderObject
	{
	public:
		static HSkybox Create(const SkyboxInitializer& init);

	public:
		virtual ~Skybox();

		virtual SPtr<rt::RenderObject> CreateRenderObject() const override;
		SPtr<rt::Skybox> GetRenderObject() const { return DPCast(rt::Skybox)(mRenderObject); }

		HSkybox GetHandler() const { return mMyHandler; }

		virtual void Destroy() override;

		void SetTexture(const RPtr<Texture>& array);
		void SetTexture(const RPtr<Texture>& right, const RPtr<Texture>& left,
			const RPtr<Texture>& top, const RPtr<Texture>& bottom,
			const RPtr<Texture>& back, const RPtr<Texture>& front);

	private:
		Skybox(const SkyboxInitializer& init);

		mutable RPtr<Texture> mTextureArray = nullptr;

		mutable RPtr<Texture> mTextureRight = nullptr;
		mutable RPtr<Texture> mTextureLeft = nullptr;
		mutable RPtr<Texture> mTextureTop = nullptr;
		mutable RPtr<Texture> mTextureBottom = nullptr;
		mutable RPtr<Texture> mTextureBack = nullptr;
		mutable RPtr<Texture> mTextureFront = nullptr;
	};

	namespace rt
	{
		class Skybox : public RenderObject
		{
		public:
			virtual ~Skybox() {}
			virtual void Initialize() override;
			virtual void Destroy() override;

			void SyncTexture(const SPtr<rt::Texture>& array);
			void SyncTexture(rt::Texture& right, rt::Texture& left,
							 rt::Texture& top, rt::Texture& bottom,
							 rt::Texture& back, rt::Texture& front);

			SPtr<render::TextureView> GetTextureView() const { return mSkyboxTextureView; }
			SPtr<render::Sampler> GetSampler() const { return mSkyboxSampler; }

		private:
			friend class cube::Skybox;

			Skybox();

			void UpdateTextureArray();
			void UpdateTextures();

			SPtr<Texture> mTextureArray;

			Texture* mpTextureRight;
			Texture* mpTextureLeft;
			Texture* mpTextureTop;
			Texture* mpTextureBottom;
			Texture* mpTextureBack;
			Texture* mpTextureFront;
			SPtr<render::Texture> mCombinedTexture;

			SPtr<render::TextureView> mSkyboxTextureView;
			SPtr<render::Sampler> mSkyboxSampler;
		};
	} // namespace rt
} // namespace cube
