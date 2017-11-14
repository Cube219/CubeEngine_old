#pragma once

#include "../BaseRenderAPIHeader.h"

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

		struct BaseRenderSubpass
		{
			struct AttachmentRef
			{
				uint32_t index;
				ImageLayout layout;
			};
			Vector<AttachmentRef> mInputs;
			Vector<AttachmentRef> mColors;
			AttachmentRef mDepthStencil;
		};
		struct BaseRenderSubpassDependency
		{
			uint32_t srcIndex;
			uint32_t dstIndex;
			PipelineStageBits srcStageMask;
			PipelineStageBits dstStageMask;
			AccessBits srcAccessMask;
			AccessBits dstAccessMask;
		};

		// ----------------------------------------------------
		//                 BaseRenderRenderPass
		// ----------------------------------------------------
		
		struct BaseRenderRenderPassInitializer
		{
			struct Attachment
			{
				SPtr<BaseRenderImageView> imageView;
				DataFormat format;
				LoadOperator loadOp;
				StoreOperator storeOp;
				Color clearColor;
				ImageLayout initialLayout;
				ImageLayout finalLayout;

				bool isDepthStencil;
				LoadOperator stencilLoadOp;
				StoreOperator stencilStoreOp;
				DepthStencilValue clearDepthStencil;
			};
			Vector<Attachment> attachments;

			struct SwapchainAttachment
			{
				SPtr<BaseRenderSwapchain> swapchain;
				LoadOperator loadOp;
				StoreOperator storeOp;
				Color clearColor;
				ImageLayout initialLayout;
				ImageLayout finalLayout;
			};
			bool hasSwapchain;
			SwapchainAttachment swapchainAttachment;

			Vector<BaseRenderSubpass> subpasses;
			Vector<BaseRenderSubpassDependency> subpassDependencies;
		};

		class BaseRenderRenderPass
		{
		public:
			virtual ~BaseRenderRenderPass(){ }

		protected:
			BaseRenderRenderPass(){ }
		};
	}
}