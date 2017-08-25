#pragma once

#include "..\BaseRenderAPIHeader.h"

namespace cube
{
	namespace core
	{
		enum class LoadOperator
		{
			Load,
			Clear,
			DontCare
		};

		enum class StoreOperator
		{
			Store,
			DontCare
		};

		struct BaseRenderSubpass; // Defined at BaseRenderAPIHeader.h

		// ----------------------------------------------------
		//                 BaseRenderRenderPass
		// ----------------------------------------------------

		class BaseRenderRenderPass
		{
		public:
			virtual ~BaseRenderRenderPass(){ }

			virtual void AddAttachment(SPtr<BaseRenderImageView>& imageView, DataFormat format, bool isDepthStencil,
				LoadOperator loadOp, StoreOperator storeOp,
				LoadOperator stencilLoadOp, StoreOperator stencilStoreOp, Color clearColor,
				ImageLayout initialLayout, ImageLayout finalLayout, DepthStencilValue clearDepthStencil) = 0;

			virtual void SetSwapchain(SPtr<BaseRenderSwapchain>& swapchain,
				LoadOperator loadOp, StoreOperator storeOp, Color clearColor,
				ImageLayout initialLayout, ImageLayout finalLayout) = 0;

			virtual void AddSubpass(BaseRenderSubpass subpass) = 0;

			virtual void Create() = 0;

		protected:
			BaseRenderRenderPass(){ }
		};
	}
}