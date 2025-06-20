#pragma once

#include "../RenderAPIHeader.h"

#include "../Utilities/BaseRenderObject.h"
#include "RenderAPI/Interface/TextureView.h"

namespace cube
{
	namespace render
	{
		struct TextureData
		{
			const void* pData = nullptr;
			Uint64 size;
		};

		struct TextureAttribute : public BaseAttribute
		{
			ResourceUsage usage;
			TextureType type;
			TextureFormat format;
			TextureBindTypeFlags bindTypeFlags;

			TextureData textureData;
			Uint32 width;
			Uint32 height;
			Uint32 depth;
			Uint32 arraySize;

			Uint32 mipLevels;

			bool isDedicated = false;
		};

		class Texture : public BaseRenderObject
		{
		public:
			Texture(const TextureAttribute& attr) :
				BaseRenderObject(attr.debugName),
				mUsage(attr.usage), mType(attr.type), mFormat(attr.format), mBindTypeFlags(attr.bindTypeFlags),
				mWidth(attr.width), mHeight(attr.height), mDepth(attr.depth), mArraySize(attr.arraySize),
				mMipLevels(attr.mipLevels)
			{}
			virtual ~Texture() {}

			ResourceUsage GetUsage() const { return mUsage; }
			TextureType GetType() const { return mType; }
			TextureFormat GetFormat() const { return mFormat; }
			TextureBindTypeFlags GetBindTypeFlags() const { return mBindTypeFlags; }
			Uint32 GetWidth() const { return mWidth; }
			Uint32 GetHeight() const { return mHeight; }
			Uint32 GetDepth() const { return mDepth; }
			Uint32 GetArraySize() const { return mArraySize; }
			Uint32 GetMipLevels() const { return mMipLevels; }

			virtual void UpdateData(CommandList& cmdList, const void* pData, Uint64 size, Uint32 width, Uint32 height) = 0;
			virtual void CopyFromTexture(CommandList& cmdList, Texture& src, Uint32 srcMipLevel, Uint32 srcArrayIndex, const Rect3D* pSrcArea,
				Uint32 dstMipLevel, Uint32 dstArrayIndex, Uint32 dstX, Uint32 dstY, Uint32 dstZ) = 0;

			virtual SPtr<TextureView> CreateView(const TextureViewAttribute& attr) = 0;

			TextureViewAttribute GetDefaultViewAttr() const { return mDefaultViewAttr; }

		protected:
			ResourceUsage mUsage;
			TextureType mType;
			TextureFormat mFormat;
			TextureBindTypeFlags mBindTypeFlags;
			Uint32 mWidth;
			Uint32 mHeight;
			Uint32 mDepth;
			Uint32 mArraySize;
			Uint32 mMipLevels;

			TextureViewAttribute mDefaultViewAttr;
		};
	} // namespace render
} // namespace cube
