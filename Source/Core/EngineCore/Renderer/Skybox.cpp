#include "Skybox.h"

#include "../EngineCore.h"
#include "RendererManager.h"
#include "Texture.h"

namespace cube
{
	HSkybox Skybox::Create(const SkyboxInitializer& init)
	{
		auto& rendererManager = ECore().GetRendererManager();

		UPtr<Skybox> skybox(new Skybox(init));
		skybox->Initialize();

		return rendererManager.RegisterSkybox(std::move(skybox));
	}

	Skybox::Skybox(const SkyboxInitializer& init)
	{
	}

	Skybox::~Skybox()
	{
	}

	SPtr<rt::RenderObject> Skybox::CreateRenderObject() const
	{
		SPtr<rt::Skybox> skybox_rt(new rt::Skybox());

		return skybox_rt;
	}

	void Skybox::SetTexture(const RPtr<Texture>& array)
	{
		RenderingThread::QueueSyncTask([this, array_rt = array->GetRenderObject()]() {
			GetRenderObject()->SyncTexture(array_rt);
		});
	}

	void Skybox::SetTexture(const RPtr<Texture>& right, const RPtr<Texture>& left,
		const RPtr<Texture>& top, const RPtr<Texture>& bottom,
		const RPtr<Texture>& back, const RPtr<Texture>& front)
	{
		RenderingThread::QueueSyncTask([this,
			right_rt = right->GetRenderObject(), left_rt = left->GetRenderObject(),
			top_rt = top->GetRenderObject(), bottom_rt = bottom->GetRenderObject(),
			back_rt = back->GetRenderObject(), front_rt = front->GetRenderObject()]() {

			GetRenderObject()->SyncTexture(*right_rt, *left_rt, *top_rt, *bottom_rt, *back_rt, *front_rt);
		});
	}

	namespace rt
	{
		Skybox::Skybox()
		{
		}

		void Skybox::Initialize()
		{
		}

		void Skybox::Destroy()
		{
		}

		void Skybox::SyncTexture(const SPtr<rt::Texture>& array)
		{
			mTextureArray = array;

			mpTextureRight = nullptr;
			mpTextureLeft = nullptr;
			mpTextureTop = nullptr;
			mpTextureBottom= nullptr;
			mpTextureBack = nullptr;
			mpTextureFront = nullptr;

			RenderingThread::QueueTask([this]() {
				UpdateTextureArray();
			});
		}

		void Skybox::SyncTexture(rt::Texture& right, rt::Texture& left,
                                 rt::Texture& top, rt::Texture& bottom,
                                 rt::Texture& back, rt::Texture& front)
		{
			mTextureArray = nullptr;

			mpTextureRight = &right;
			mpTextureLeft = &left;
			mpTextureTop = &top;
			mpTextureBottom = &bottom;
			mpTextureBack = &back;
			mpTextureFront = &front;

			RenderingThread::QueueTask([this]() {
				UpdateTextures();
			});
		}

		void Skybox::UpdateTextureArray()
		{
			mSkyboxTextureView = mTextureArray->GetTextureView();
		}

		void Skybox::UpdateTextures()
		{
			SPtr<render::Device> device = ECore().GetRendererManager().GetDevice();
			SPtr<render::Texture> faceTexture = mpTextureRight->GetTexture();

			render::TextureAttribute textureAttr;
			textureAttr.usage = render::ResourceUsage::Default;
			textureAttr.type = render::TextureType::TextureCube;
			textureAttr.format = faceTexture->GetFormat();
			textureAttr.bindTypeFlags = render::TextureBindTypeFlag::ShaderResource;
			textureAttr.textureData.pData = nullptr;
			textureAttr.textureData.size = 0;
			textureAttr.width = faceTexture->GetWidth();
			textureAttr.height = faceTexture->GetHeight();
			textureAttr.depth = 1;
			textureAttr.arraySize = 6;
			textureAttr.mipLevels = 1;

			mCombinedTexture = device->CreateTexture(textureAttr);

			render::TextureViewAttribute viewAttr = mCombinedTexture->GetDefaultViewAttr();
			mSkyboxTextureView = mCombinedTexture->CreateView(viewAttr);

			render::CommandListAttribute cmdListAttr;
			cmdListAttr.usage = render::CommandListUsage::TransferImmediate;
			cmdListAttr.threadIndex = 0;
			cmdListAttr.isSub = false;
			SPtr<render::CommandList> cmdList = device->GetCommandList(cmdListAttr);

			cmdList->Begin();
			
			mCombinedTexture->CopyFromTexture(*cmdList, *(mpTextureRight->GetTexture()), 1, 0, nullptr, 1, 0, 0, 0, 0);
			mCombinedTexture->CopyFromTexture(*cmdList, *(mpTextureRight->GetTexture()), 1, 0, nullptr, 1, 1, 0, 0, 0);
			mCombinedTexture->CopyFromTexture(*cmdList, *(mpTextureRight->GetTexture()), 1, 0, nullptr, 1, 2, 0, 0, 0);
			mCombinedTexture->CopyFromTexture(*cmdList, *(mpTextureRight->GetTexture()), 1, 0, nullptr, 1, 3, 0, 0, 0);
			mCombinedTexture->CopyFromTexture(*cmdList, *(mpTextureRight->GetTexture()), 1, 0, nullptr, 1, 4, 0, 0, 0);
			mCombinedTexture->CopyFromTexture(*cmdList, *(mpTextureRight->GetTexture()), 1, 0, nullptr, 1, 5, 0, 0, 0);

			cmdList->End();

			device->SubmitCommandList(cmdList);
		}
	}
	// namespace rt
} // namespace cube
