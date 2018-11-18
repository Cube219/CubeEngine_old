#pragma once

#include "../RenderAPIHeader.h"

#include "RenderTypes.h"

namespace cube
{
	namespace render
	{
		struct SwapChainAttribute
		{
			Uint32 width;
			Uint32 height;

			TextureFormat colorBufferFormat;
			TextureFormat depthBufferFormat;

			bool vsync;

			Uint32 bufferCount;

			const char* debugName = nullptr;
		};

		class SwapChain
		{
		public:
			SwapChain() {}
			virtual ~SwapChain() {}

			virtual void Present() = 0;
			virtual void Resize(Uint32 width, Uint32 height, bool vsync) = 0;

			virtual void SetFullscreenMode() = 0;
			virtual void SetWindowedMode() = 0;
		};
	} // namespace render
} // namespace cube
