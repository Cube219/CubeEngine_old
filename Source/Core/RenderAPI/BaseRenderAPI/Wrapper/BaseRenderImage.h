#pragma once

#include "../BaseRenderAPIHeader.h"

namespace cube
{
	namespace core
	{
		enum class ImageType
		{
			Image1D,
			Image2D,
			Image3D
		};
		enum class ImageViewType
		{
			Image1D,
			Image2D,
			Image3D,
			Cube,
			Image1DArray,
			Image2DArray,
			CubeArray
		};
		enum class ImageUsageBits
		{
			TransferSourceBit = 1,
			TransferDestinationBit = 2,
			SampledBit = 4,
			StorageBit = 8,
			ColorAttachmentBit = 16,
			DepthStencilAttachmentBit = 32,
			TransientAttachmentBit = 64,
			InputAttachmentBit = 128,
		};
		SET_ENUM_AS_FLAGS(ImageUsageBits)
		enum class ImageAspectBits
		{
			Color = 1,
			Depth = 2,
			Stencil = 4,
			Metadata = 8
		};
		SET_ENUM_AS_FLAGS(ImageAspectBits)
		enum class ImageLayout
		{
			Undefined,
			General,
			ColorAttachmentOptimal,
			DepthStencilAttachmentOptimal,
			DepthStencilReadOnlyOptimal,
			ShaderReadOnlyOptimal,
			TransferSourceOptimal,
			TransferDestinationOptimal,
			Preinitialized,
			PresentSource,
			SharedPresent
		};

		class BaseRenderImageView
		{
		public:
			virtual ~BaseRenderImageView(){ }

		protected:
			BaseRenderImageView(){ }

		};

		struct BaseRenderImageInitializer
		{
			ImageType type;
			DataFormat format;
			uint32_t width;
			uint32_t height;
			uint32_t depth;
			uint32_t mipLevels;
			ImageUsageBits usage;
		};

		class BaseRenderImage
		{
		public:
			virtual ~BaseRenderImage(){ }

			virtual SPtr<BaseRenderImageView> GetImageView(DataFormat format, ImageAspectBits aspectBits, ImageViewType type) = 0;

		protected:
			BaseRenderImage(){ }
		};
	}
}