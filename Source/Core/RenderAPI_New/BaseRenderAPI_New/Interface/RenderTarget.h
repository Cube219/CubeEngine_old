﻿#pragma once

#include "../RenderAPIHeader.h"

namespace cube
{
	namespace render
	{
		struct RenderTargetAttribute
		{
			SPtr<TextureView> textureView;
			bool isSwapChain;
			SPtr<SwapChain> swapChain;

			LoadOperator loadOp;
			StoreOperator storeOp;

			LoadOperator stencilLoadOp;
			StoreOperator stencilStoreOp;

			float defaultColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
			float defaultDepth = 1.0f;
			Uint32 defaultStencil = 0;
		};

		class RenderTarget
		{
		public:
			RenderTarget() {}
			virtual ~RenderTarget() {}
		};
	} // namespace render
} // namespace cube