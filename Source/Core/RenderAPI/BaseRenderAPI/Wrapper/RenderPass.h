#pragma once

#include "../RenderAPIHeader.h"

namespace cube
{
	namespace render
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

		struct Subpass
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
		struct SubpassDependency
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
		
		struct RenderPassInitializer
		{
			struct Attachment
			{
				SPtr<ImageView> imageView;
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
				SPtr<Swapchain> swapchain;
				LoadOperator loadOp;
				StoreOperator storeOp;
				Color clearColor;
				ImageLayout initialLayout;
				ImageLayout finalLayout;
			};
			bool hasSwapchain;
			SwapchainAttachment swapchainAttachment;

			Vector<Subpass> subpasses;
			Vector<SubpassDependency> subpassDependencies;
		};

		class RenderPass
		{
		public:
			virtual ~RenderPass(){ }

		protected:
			RenderPass(){ }
		};
	} // namespace render
} // namespace cube
