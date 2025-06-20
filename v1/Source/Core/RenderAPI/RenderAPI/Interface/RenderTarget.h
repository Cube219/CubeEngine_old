#pragma once

#include "../RenderAPIHeader.h"

#include "../Utilities/BaseRenderObject.h"

namespace cube
{
	namespace render
	{
		struct RenderTargetAttribute : public BaseAttribute
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

		class RenderTarget : public BaseRenderObject
		{
		public:
			RenderTarget(const RenderTargetAttribute& attr) :
				BaseRenderObject(attr.debugName)
			{}
			virtual ~RenderTarget() {}
		};
	} // namespace render
} // namespace cube
