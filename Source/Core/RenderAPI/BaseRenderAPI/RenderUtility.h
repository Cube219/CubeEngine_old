#pragma once

#include <stdint.h>
#include "Base/BaseTypes.h"

namespace cube
{
	namespace render
	{
		struct Viewport
		{
			float x;
			float y;
			float width;
			float height;
			float minDepth;
			float maxDepth;
		};

		struct Rect2D
		{
			int x;
			int y;
			int width;
			int height;
		};

		enum class PipelineType
		{
			Graphics, Compute
		};

		enum class ShaderLanguage
		{
			GLSL,
			HLSL,
			SPIR_V
		};

		enum class ShaderTypeBits
		{
			Vertex = 1,
			Pixel = 1 << 1,
			Fragment = 1 << 2,
			Geometry = 1 << 3,
			TessellationControl = 1 << 4,
			TessellationEvaluation = 1 << 5,
			Compute = 1 << 6
		};
		SET_ENUM_AS_FLAGS(ShaderTypeBits)

			enum class DataFormat
		{
			R8G8B8A8_Unorm,
			R32G32_SFloat,
			R32G32B32_SFloat,
			R32G32B32A32_SFloat,
			D16_Unorm
		};

		enum class VertexTopology
		{
			Point,
			Line,
			Triangle
		};

		enum class PolygonMode
		{
			Point,
			Line,
			Fill
		};

		enum class PolygonFrontFace
		{
			Clockwise,
			CounterClockwise
		};

		enum class CullMode
		{
			None,
			Front,
			Back,
			FrontBack
		};

		enum class CompareOperator
		{
			Never,
			Always,
			Equal,
			NotEqual,
			Less,
			Greater,
			LessOrEqual,
			GreaterOrEqual
		};

		enum class StencilOperator
		{
			Keep,
			Zero,
			Replace,
			IncrementAndClamp,
			DecrementAndClamp,
			Invert,
			IncrementAndWrap,
			DecrementAndWrap
		};

		struct StencilOperatorState
		{
			StencilOperator failOperator;
			StencilOperator passOperator;
			CompareOperator compareOperator;
			uint32_t compareMask;
			uint32_t reference;
			StencilOperator depthFailOperator;
			uint32_t writeMask;
		};

		// TODO: 나머지 Factor들은 Blend 알고 나서 추가
		enum class BlendFactor
		{
			Zero,
			One,
			SourceColor,
			OneMinusSourceColor,
			DestinationColor,
			OneMinusDestinationColor,
			SourceAlpha,
			OneMinusSourceAlpha,
			DestinationAlpha,
			OneMinusDestinationAlpha,
			ConstantColor,
			OneMinusConstantColor,
			ConstantAlpha,
			OneMinusConstantAlpha
		};

		enum class BlendOperator
		{
			Add,
			Subtract,
			ReverseSubtract,
			Min,
			Max
		};

		enum class LogicOperator
		{
			Clear,
			And,
			AndReverse,
			Copy,
			AndInverted,
			NoOp,
			Xor,
			Or,
			Nor,
			Equivalent,
			Invert,
			OrReverse,
			CopyInverted,
			OrInverted,
			Nand,
			Set
		};

		union Color
		{
			struct Float32
			{
				float r;
				float g;
				float b;
				float a;
			};
			struct Int32
			{
				int32_t r;
				int32_t g;
				int32_t b;
				int32_t a;
			};
			struct Uint32
			{
				uint32_t r;
				uint32_t g;
				uint32_t b;
				uint32_t a;
			};

			Float32 float32;
			Int32 int32;
			Uint32 uint32;
		};

		struct DepthStencilValue
		{
			float depth;
			uint32_t stencil;
		};

		enum class AccessBits
		{
			None = 0,
			IndirectCommandReadBit = 1,
			IndexReadBit = 1 << 1,
			VertexAttributeReadBit = 1 << 2,
			UniformReadBit = 1 << 3,
			InputAttachmentReadBit = 1 << 4,
			ShaderReadBit = 1 << 5,
			ShaderWriteBit = 1 << 6,
			ColorAttachmentReadBit = 1 << 7,
			ColorAttachmentWriteBit = 1 << 8,
			DepthStencilAttachmentReadBit = 1 << 9,
			DepthStencilAttachmentWriteBit = 1 << 10,
			TransferReadBit = 1 << 11,
			TransferWriteBit = 1 << 12,
			HostReadBit = 1 << 13,
			HostWriteBit = 1 << 14,
			MemoryReadBit = 1 << 15,
			MemoryWriteBit = 1 << 16
		};
		SET_ENUM_AS_FLAGS(AccessBits)
	} // namespace render
} // namespace cube
