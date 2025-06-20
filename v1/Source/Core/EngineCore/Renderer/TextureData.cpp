#include "TextureData.h"

#include "../Assertion.h"

namespace cube
{
	TextureData::TextureData(Uint32 width, Uint32 height, Uint32 depth) : 
		mWidth(width), mHeight(height), mDepth(depth),
		mSize(width * height * depth * 4)
	{
		mData = malloc(mSize);
	}

	TextureData::~TextureData()
	{
		free(mData);
	}

	void TextureData::SetPixels(void* pPixels, Uint64 numPixels)
	{
		CHECK(mSize == numPixels * 4, "Size don't match. (Expected: {0}, Actual: {1})", mSize / 4, numPixels);

		memcpy(mData, pPixels, numPixels * 4);
	}
} // namespace cube
