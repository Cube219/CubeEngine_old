#include "Skybox.h"

#include "../../EngineCore.h"
#include "../RendererManager.h"
#include "../Texture.h"

namespace cube
{
	HSkybox Skybox::Create(const SkyboxInitializer& init)
	{
		auto& rendererManager = ECore().GetRendererManager();

		UPtr<Skybox> skybox(new Skybox(init));
		skybox->Initialize();

		return rendererManager.RegisterSkybox(std::move(skybox));
	}

	Skybox::Skybox(const SkyboxInitializer& init) : 
		mTextureArray(init.textureArray),
		mTextureRight(init.textureRight),
		mTextureLeft(init.textureLeft),
		mTextureTop(init.textureTop),
		mTextureBottom(init.textureBottom),
		mTextureBack(init.textureBack),
		mTextureFront(init.textureFront)
	{
	}

	Skybox::~Skybox()
	{
	}

	SPtr<rt::RenderObject> Skybox::CreateRenderObject() const
	{
		SPtr<rt::Skybox> skybox_rt(new rt::Skybox());

		RenderingThread::QueueSyncTask([this]() {
			if(mTextureArray != nullptr) {
				GetRenderObject()->SyncTexture(mTextureArray->GetRenderObject());

				mTextureArray = nullptr;
			} else {
				SPtr<rt::Texture> right_rt = mTextureRight->GetRenderObject();
				SPtr<rt::Texture> left_rt = mTextureLeft->GetRenderObject();
				SPtr<rt::Texture> top_rt = mTextureTop->GetRenderObject();
				SPtr<rt::Texture> bottom_rt = mTextureBottom->GetRenderObject();
				SPtr<rt::Texture> back_rt = mTextureBack->GetRenderObject();
				SPtr<rt::Texture> front_rt = mTextureFront->GetRenderObject();

				GetRenderObject()->SyncTexture(*right_rt, *left_rt, *top_rt, *bottom_rt, *back_rt, *front_rt);

				mTextureRight = nullptr;
				mTextureLeft = nullptr;
				mTextureTop = nullptr;
				mTextureBottom = nullptr;
				mTextureBack= nullptr;
				mTextureFront= nullptr;
			}
		});

		return skybox_rt;
	}

	void Skybox::Destroy()
	{
		UPtr<Skybox> ptr = ECore().GetRendererManager().UnregisterSkybox(GetHandler());

		RenderObject::Destroy();
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
			SPtr<render::Device> device = ECore().GetRendererManager().GetDevice();

			using namespace render;

			SamplerAttribute samplerAttr;
			samplerAttr.minFilter = FilterType::Linear;
			samplerAttr.magFilter = FilterType::Linear;
			samplerAttr.mipMapFilter = FilterType::Linear;
			samplerAttr.addressU = AddressMode::Clamp;
			samplerAttr.addressV = AddressMode::Clamp;
			samplerAttr.addressW = AddressMode::Clamp;
			samplerAttr.maxAnisotropy = 16;
			samplerAttr.mipLodBias = 0;
			samplerAttr.minLod = 0;
			samplerAttr.maxLod = 0;
			samplerAttr.borderColor[0] = 0.0f;
			samplerAttr.borderColor[1] = 0.0f;
			samplerAttr.borderColor[2] = 0.0f;
			samplerAttr.borderColor[3] = 1.0f;
			samplerAttr.debugName = "Sampler";

			mSkyboxSampler = device->CreateSampler(samplerAttr);
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
			textureAttr.debugName = "skybox textures";

			mCombinedTexture = device->CreateTexture(textureAttr);

			render::TextureViewAttribute viewAttr = mCombinedTexture->GetDefaultViewAttr();
			mSkyboxTextureView = mCombinedTexture->CreateView(viewAttr);

			render::CommandListAttribute cmdListAttr;
			cmdListAttr.usage = render::CommandListUsage::TransferImmediate;
			cmdListAttr.threadIndex = 0;
			cmdListAttr.isSub = false;
			SPtr<render::CommandList> cmdList = device->GetCommandList(cmdListAttr);

			cmdList->Begin();
			
			mCombinedTexture->CopyFromTexture(*cmdList, *(mpTextureRight->GetTexture()), 0, 0, nullptr, 0, 0, 0, 0, 0);
			mCombinedTexture->CopyFromTexture(*cmdList, *(mpTextureLeft->GetTexture()), 0, 0, nullptr, 0, 1, 0, 0, 0);
			mCombinedTexture->CopyFromTexture(*cmdList, *(mpTextureTop->GetTexture()), 0, 0, nullptr, 0, 2, 0, 0, 0);
			mCombinedTexture->CopyFromTexture(*cmdList, *(mpTextureBottom->GetTexture()), 0, 0, nullptr, 0, 3, 0, 0, 0);
			mCombinedTexture->CopyFromTexture(*cmdList, *(mpTextureBack->GetTexture()), 0, 0, nullptr, 0, 4, 0, 0, 0);
			mCombinedTexture->CopyFromTexture(*cmdList, *(mpTextureFront->GetTexture()), 0, 0, nullptr, 0, 5, 0, 0, 0);

			cmdList->End();

			device->SubmitCommandList(cmdList);

			RenderingThread::QueueTask([cmdList]() mutable {
				cmdList.reset();
			});
		}
	}
	// namespace rt
} // namespace cube
