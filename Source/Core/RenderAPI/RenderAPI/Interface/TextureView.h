#pragma once

#include "../RenderAPIHeader.h"

#include "../Utilities/BaseRenderObject.h"

namespace cube
{
	namespace render
	{
		struct TextureViewAttribute : public BaseAttribute
		{
			TextureViewType type;
			TextureFormat format;
			TextureViewComponentFlags componentFlags;

			Uint32 firstMipLevel;
			Uint32 numMipLevels;
			Uint32 firstArrayIndex;
			Uint32 numArray;
		};

		class TextureView : public BaseRenderObject
		{
		public:
			TextureView(const TextureViewAttribute& attr) :
				BaseRenderObject(attr.debugName)
			{}
			virtual ~TextureView() {}

			TextureViewType GetType() const { return mType; }
			TextureFormat GetFormat() const { return mFormat; }
			TextureViewComponentFlags GetComponentFlags() const { return mComponentFlags; }
			Uint32 GetFirstMipLevel() const { return mFirstMipLevel; }
			Uint32 GetNumMipLevels() const { return mNumMipLevels; }
			Uint32 GetFirstArrayIndex() const { return mFirstArrayIndex; }
			Uint32 GetNumArray() const { return mNumArray; }

		protected:
			TextureViewType mType;
			TextureFormat mFormat;
			TextureViewComponentFlags mComponentFlags;
			Uint32 mFirstMipLevel;
			Uint32 mNumMipLevels;
			Uint32 mFirstArrayIndex;
			Uint32 mNumArray;
		};
	} // namespace render
} // namespace cube
