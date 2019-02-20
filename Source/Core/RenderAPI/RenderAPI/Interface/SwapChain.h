#pragma once

#include "../RenderAPIHeader.h"

#include "../Utilities/BaseRenderObject.h"
#include "RenderTypes.h"

namespace cube
{
	namespace render
	{
		struct SwapChainAttribute : public BaseAttribute
		{
			Uint32 width;
			Uint32 height;

			TextureFormat colorBufferFormat;
			TextureFormat depthBufferFormat;

			bool vsync;

			Uint32 bufferCount;
		};

		class SwapChain : public BaseRenderObject
		{
		public:
			SwapChain(const char* debugName) : BaseRenderObject(debugName) {}
			virtual ~SwapChain() {}

			virtual void AcquireNextImage() = 0;
			virtual void Present() = 0;

			virtual void Resize(Uint32 width, Uint32 height, bool vsync) = 0;

			virtual void SetFullscreenMode() = 0;
			virtual void SetWindowedMode() = 0;
		};
	} // namespace render
} // namespace cube
