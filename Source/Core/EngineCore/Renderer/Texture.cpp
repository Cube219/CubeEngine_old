#include "Texture.h"

#include "../EngineCore.h"
#include "../Resource/ResourceManager.h"
#include "TextureData.h"

namespace cube
{
	RPtr<Texture> Texture::Load(StringRef path)
	{
		auto texture = ECore().GetResourceManager().LoadResource<Texture>(path);
		texture->Initialize();

		return texture;
	}

	Texture::Texture() :
		mTextureData(nullptr)
	{
	}

	Texture::Texture(const SPtr<TextureData>& initialData) :
		mTextureData(initialData)
	{
	}

	Texture::~Texture()
	{
	}

	SPtr<rt::RenderObject> Texture::CreateRenderObject() const
	{
		SPtr<rt::Texture> texture_rt(new rt::Texture(mTextureData));

		mTextureData = nullptr;

		return texture_rt;
	}

	namespace rt
	{
		Texture::Texture(const SPtr<TextureData>& initialData) :
			mTextureData(initialData)
		{
		}

		Texture::~Texture()
		{
		}

		void Texture::Initialize()
		{
			if(mTextureData != nullptr) {
				FlushToRenderTexture();
			}
		}

		void Texture::SyncTextureData(const SPtr<TextureData>& textureData)
		{
			mTextureData = textureData;

			RenderingThread::QueueTask([this]() {
				FlushToRenderTexture();
			});
		}

		void Texture::FlushToRenderTexture()
		{
			auto device = ECore().GetRendererManager().GetDevice();

			using namespace render;

			TextureAttribute attr;
			attr.usage = ResourceUsage::Default;
			attr.type = TextureType::Texture2D;
			attr.format = TextureFormat::RGBA_8_UNorm;
			attr.bindTypeFlags = TextureBindTypeFlag::ShaderResource;
			attr.textureData.pData = mTextureData->GetData();
			attr.textureData.size = mTextureData->GetSize();
			attr.width = mTextureData->GetWidth();
			attr.height = mTextureData->GetHeight();
			attr.depth = mTextureData->GetDepth();
			attr.arraySize = 1;
			attr.mipLevels = 1;
			attr.isDedicated = false;
			attr.debugName = "Texture";

			mTexture = device->CreateTexture(attr);
			mTextureView = mTexture->CreateView(mTexture->GetDefaultViewAttr());

			SamplerAttribute samplerAttr;
			samplerAttr.minFilter = FilterType::Linear;
			samplerAttr.magFilter = FilterType::Linear;
			samplerAttr.mipMapFilter = FilterType::Linear;
			samplerAttr.addressU = AddressMode::Wrap;
			samplerAttr.addressV = AddressMode::Wrap;
			samplerAttr.addressW = AddressMode::Wrap;
			samplerAttr.maxAnisotropy = 16;
			samplerAttr.mipLodBias = 0;
			samplerAttr.minLod = 0;
			samplerAttr.maxLod = 0;
			samplerAttr.borderColor[0] = 0.0f;
			samplerAttr.borderColor[1] = 0.0f;
			samplerAttr.borderColor[2] = 0.0f;
			samplerAttr.borderColor[3] = 1.0f;
			samplerAttr.debugName = "Sampler";

			mSampler = device->CreateSampler(samplerAttr);

			mTextureData = nullptr;
		}
	} // namespace rt
} // namespace cube
