#pragma once

#include "../EngineCoreHeader.h"

namespace cube
{
	// TOOD: 나중에 여러 pixel들에 대응
	struct TexturePixel
	{
	};

	class ENGINE_CORE_EXPORT TextureData
	{
	public:
		TextureData(Uint32 width, Uint32 height, Uint32 depth);
		~TextureData();

		TextureData(const TextureData& other) : 
			mWidth(other.mWidth),
			mHeight(other.mHeight),
			mDepth(other.mDepth),
			mSize(other.mSize)
		{
			mData = malloc(mSize);
			memcpy(mData, other.mData, mSize);
		}
		TextureData& operator=(const TextureData& rhs)
		{
			if(this == &rhs)
				return *this;

			mWidth = rhs.mWidth;
			mHeight = rhs.mHeight;
			mDepth = rhs.mDepth;

			if(mSize != rhs.mSize) {
				free(mData);
				mData = malloc(rhs.mSize);

				mSize = rhs.mSize;
			}

			memcpy(mData, rhs.mData, mSize);

			return *this;
		}

		void SetPixels(void* pPixels, Uint64 numPixels);

		Uint32 GetWidth() const { return mWidth; }
		Uint32 GetHeight() const { return mHeight; }
		Uint32 GetDepth() const { return mDepth; }
		void* GetData() const { return mData; }
		Uint64 GetSize() const { return mSize; }

	private:
		Uint32 mWidth;
		Uint32 mHeight;
		Uint32 mDepth;

		void *mData;
		Uint64 mSize;
	};
} // namespace cube
