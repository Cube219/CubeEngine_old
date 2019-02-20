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
			TextureView(const char* debugName) : BaseRenderObject(debugName) {}
			virtual ~TextureView() {}
		};
	} // namespace render
} // namespace cube
