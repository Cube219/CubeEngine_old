#pragma once

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
		};

		class RenderTarget
		{
		public:
			RenderTarget() {}
			virtual ~RenderTarget() {}
		};
	} // namespace render
} // namespace cube
