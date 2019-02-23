#include "Texture.h"

#include "../EngineCore.h"
#include "../Resource/ResourceManager.h"

namespace cube
{
	RPtr<Texture> Texture::Load(StringRef path)
	{
		return ECore().GetResourceManager().LoadResource<Texture>(path);
	}

	void Texture::WriteData(SPtr<render::Device>& device, void* data, Uint64 size, Uint32 width, Uint32 height)
	{
		using namespace render;

		TextureAttribute attr;
		attr.usage = ResourceUsage::Default;
		attr.type = TextureType::Texture2D;
		attr.format = TextureFormat::RGBA_8_UNorm;
		attr.bindTypeFlags = TextureBindTypeFlagBits::ShaderResource_Bit;
		attr.textureData.pData = data;
		attr.textureData.size = size;
		attr.width = width;
		attr.height = height;
		attr.depth = 1;
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
	}
} // namespace cube
