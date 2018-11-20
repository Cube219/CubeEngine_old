#pragma once

#include "../RenderAPIHeader.h"

namespace cube
{
	namespace render
	{
		struct TextureViewAttribute
		{
			TextureViewType type;
			TextureFormat format;
			TextureViewComponentFlags componentFlags;

			Uint32 firstMipLevel;
			Uint32 numMipLevels;
			Uint32 firstArrayIndex;
			Uint32 numArray;

			const char* debugName = "";
		};

		class TextureView
		{
		public:
			TextureView() {}
			virtual ~TextureView() {}
		};
	} // namespace render
} // namespace cube
