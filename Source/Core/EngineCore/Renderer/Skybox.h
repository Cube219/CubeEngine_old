#pragma once

#include "../EngineCoreHeader.h"

#include "RenderObject.h"
#include "RenderAPI/Interface/Texture.h"
#include "RenderAPI/Interface/TextureView.h"
#include "RenderAPI/Interface/Sampler.h"

namespace cube
{
	struct SkyboxInitializer
	{
	};

	class ENGINE_CORE_EXPORT Skybox : public RenderObject
	{
	public:
		static HSkybox Create(const SkyboxInitializer& init);

	public:
		virtual ~Skybox();

		virtual SPtr<rt::RenderObject> CreateRenderObject() const override;
		SPtr<rt::Skybox> GetRenderObject() const { return DPCast(rt::Skybox)(mRenderObject); }

		void SetTexture(const RPtr<Texture>& array);
		void SetTexture(const RPtr<Texture>& right, const RPtr<Texture>& left,
			const RPtr<Texture>& top, const RPtr<Texture>& bottom,
			const RPtr<Texture>& back, const RPtr<Texture>& front);

	private:
		Skybox(const SkyboxInitializer& init);
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
		};
	} // namespace rt
} // namespace cube
