#include "VulkanUtility.h"

#include <memory.h>

#include "EngineCore/Assertion.h"

#ifdef _DEBUG

void PrintVkFail(const char* fileName, int lineNum, cube::String msg, VkResult res)
{
	cube::String errMsg;

	switch(res) {
		case VK_ERROR_INITIALIZATION_FAILED:
			errMsg = CUBE_T("(VK_ERROR_INITIALIZATION_FAILED)");
			break;
		case VK_ERROR_DEVICE_LOST:
			errMsg = CUBE_T("(VK_ERROR_DEVICE_LOST)");
			break;
		case VK_ERROR_MEMORY_MAP_FAILED:
			errMsg = CUBE_T("(VK_ERROR_MEMORY_MAP_FAILED)");
			break;
		case VK_ERROR_LAYER_NOT_PRESENT:
			errMsg = CUBE_T("(VK_ERROR_LAYER_NOT_PRESENT)");
			break;
		case VK_ERROR_EXTENSION_NOT_PRESENT:
			errMsg = CUBE_T("(VK_ERROR_EXTENSION_NOT_PRESENT)");
			break;
		case VK_ERROR_FEATURE_NOT_PRESENT:
			errMsg = CUBE_T("(VK_ERROR_FEATURE_NOT_PRESENT)");
			break;
		case VK_ERROR_INCOMPATIBLE_DRIVER:
			errMsg = CUBE_T("(VK_ERROR_INCOMPATIBLE_DRIVER)");
			break;
		case VK_ERROR_TOO_MANY_OBJECTS:
			errMsg = CUBE_T("(VK_ERROR_TOO_MANY_OBJECTS)");
			break;
		case VK_ERROR_FORMAT_NOT_SUPPORTED:
			errMsg = CUBE_T("(VK_ERROR_FORMAT_NOT_SUPPORTED)");
			break;
		case VK_ERROR_FRAGMENTED_POOL:
			errMsg = CUBE_T("(VK_ERROR_FRAGMENTED_POOL)");
			break;
		case VK_ERROR_SURFACE_LOST_KHR:
			errMsg = CUBE_T("(VK_ERROR_SURFACE_LOST_KHR)");
			break;
		case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
			errMsg = CUBE_T("(VK_ERROR_NATIVE_WINDOW_IN_USE_KHR)");
			break;
		case VK_ERROR_OUT_OF_DATE_KHR:
			errMsg = CUBE_T("(VK_ERROR_OUT_OF_DATE_KHR)");
			break;
		case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
			errMsg = CUBE_T("(VK_ERROR_INCOMPATIBLE_DISPLAY_KHR)");
			break;
		case VK_ERROR_VALIDATION_FAILED_EXT:
			errMsg = CUBE_T("(VK_ERROR_VALIDATION_FAILED_EXT)");
			break;
		case VK_ERROR_INVALID_SHADER_NV:
			errMsg = CUBE_T("(VK_ERROR_INVALID_SHADER_NV)");
			break;
		case VK_ERROR_OUT_OF_POOL_MEMORY_KHR:
			errMsg = CUBE_T("(VK_ERROR_OUT_OF_POOL_MEMORY_KHR)");
			break;
		case VK_ERROR_INVALID_EXTERNAL_HANDLE_KHR:
			errMsg = CUBE_T("(VK_ERROR_INVALID_EXTERNAL_HANDLE_KHR)");
			break;
		default:
			errMsg = CUBE_T("(UNKNOWN)");
			break;
	}

	ASSERTION_FAILED("{0} {1}", msg, errMsg);
}

#endif // _DEBUG

namespace cube
{
	namespace render
	{
		VkFormat GetVkFormat(DataFormat format)
		{
			VkFormat f;

			switch(format) {
				case DataFormat::R8G8B8A8_Unorm:
					f = VK_FORMAT_R8G8B8A8_UNORM;
					break;
				case DataFormat::R32G32_SFloat:
					f = VK_FORMAT_R32G32_SFLOAT;
					break;
				case DataFormat::R32G32B32_SFloat:
					f = VK_FORMAT_R32G32B32_SFLOAT;
					break;
				case DataFormat::R32G32B32A32_SFloat:
					f = VK_FORMAT_R32G32B32A32_SFLOAT;
					break;
				case DataFormat::D16_Unorm:
					f = VK_FORMAT_D16_UNORM;
					break;

				default:
					ASSERTION_FAILED("Unknown DataFormat ({0})", (int)format);
					break;
			}

			return f;
		}

		VkPrimitiveTopology GetVkPrimitiveTopology(VertexTopology topology)
		{
			VkPrimitiveTopology t;

			switch(topology) {
				case VertexTopology::Point:
					t = VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
					break;
				case VertexTopology::Line:
					t = VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
					break;
				case VertexTopology::Triangle:
					t = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
					break;

				default:
					ASSERTION_FAILED("Unknown VertexTopology ({0})", (int)topology);
					break;
			}

			return t;
		}

		VkViewport GetVkViewport(Viewport viewport)
		{
			VkViewport v;
			v.x = viewport.x;
			v.y = viewport.y;
			v.width = viewport.width;
			v.height = viewport.height;
			v.minDepth = viewport.minDepth;
			v.maxDepth = viewport.maxDepth;

			return v;
		}

		VkRect2D GetVkRect2D(Rect2D rect)
		{
			VkRect2D r;
			r.offset.x = rect.x;
			r.offset.y = rect.y;
			r.extent.width = rect.width;
			r.extent.height = rect.height;

			return r;
		}

		VkPolygonMode GetVkPolygonMode(PolygonMode polygonMode)
		{
			VkPolygonMode p;

			switch(polygonMode) {
				case PolygonMode::Point:
					p = VK_POLYGON_MODE_POINT;
					break;
				case PolygonMode::Line:
					p = VK_POLYGON_MODE_LINE;
					break;
				case PolygonMode::Fill:
					p = VK_POLYGON_MODE_FILL;
					break;

				default:
					ASSERTION_FAILED("Unknown PolygonMode ({0})", (int)polygonMode);
					break;
			}

			return p;
		}

		VkFrontFace GetVkFrontFace(PolygonFrontFace frontFace)
		{
			VkFrontFace f;

			switch(frontFace) {
				case PolygonFrontFace::Clockwise:
					f = VK_FRONT_FACE_CLOCKWISE;
					break;
				case PolygonFrontFace::CounterClockwise:
					f = VK_FRONT_FACE_COUNTER_CLOCKWISE;
					break;

				default:
					ASSERTION_FAILED("Unknown PolygonFrontFace ({0})", (int)frontFace);
					break;
			}

			return f;
		}

		VkCullModeFlags GetVkCullModeFlags(CullMode cullMode)
		{
			VkCullModeFlags c;

			switch(cullMode) {
				case CullMode::None:
					c = VK_CULL_MODE_NONE;
					break;
				case CullMode::Front:
					c = VK_CULL_MODE_FRONT_BIT;
					break;
				case CullMode::Back:
					c = VK_CULL_MODE_BACK_BIT;
					break;
				case CullMode::FrontBack:
					c = VK_CULL_MODE_FRONT_AND_BACK;
					break;

				default:
					ASSERTION_FAILED("Unknown CullMode ({0})", (int)cullMode);
					break;
			}

			return c;
		}

		VkCompareOp GetVkCompareOp(CompareOperator compareOperator)
		{
			VkCompareOp o;

			switch(compareOperator) {
				case CompareOperator::Never:
					o = VK_COMPARE_OP_NEVER;
					break;
				case CompareOperator::Always:
					o = VK_COMPARE_OP_ALWAYS;
					break;
				case CompareOperator::Equal:
					o = VK_COMPARE_OP_EQUAL;
					break;
				case CompareOperator::NotEqual:
					o = VK_COMPARE_OP_NOT_EQUAL;
					break;
				case CompareOperator::Less:
					o = VK_COMPARE_OP_LESS;
					break;
				case CompareOperator::Greater:
					o = VK_COMPARE_OP_GREATER;
					break;
				case CompareOperator::LessOrEqual:
					o = VK_COMPARE_OP_LESS_OR_EQUAL;
					break;
				case CompareOperator::GreaterOrEqual:
					o = VK_COMPARE_OP_GREATER_OR_EQUAL;
					break;

				default:
					ASSERTION_FAILED("Unknown CompareOperator ({0})", (int)compareOperator);
					break;
			}

			return o;
		}

		VkStencilOpState GetVkStencilOpState(StencilOperatorState operatorState)
		{
			VkStencilOpState opState;
			opState.failOp = GetVkStencilOp(operatorState.failOperator);
			opState.passOp = GetVkStencilOp(operatorState.passOperator);
			opState.compareOp = GetVkCompareOp(operatorState.compareOperator);
			opState.compareMask = operatorState.compareMask;
			opState.reference = operatorState.reference;
			opState.depthFailOp = GetVkStencilOp(operatorState.depthFailOperator);
			opState.writeMask = operatorState.writeMask;

			return opState;
		}

		VkStencilOp GetVkStencilOp(StencilOperator op)
		{
			VkStencilOp o;

			switch(op) {
				case StencilOperator::Keep:
					o = VK_STENCIL_OP_KEEP;
					break;
				case StencilOperator::Zero:
					o = VK_STENCIL_OP_ZERO;
					break;
				case StencilOperator::Replace:
					o = VK_STENCIL_OP_REPLACE;
					break;
				case StencilOperator::IncrementAndClamp:
					o = VK_STENCIL_OP_INCREMENT_AND_CLAMP;
					break;
				case StencilOperator::DecrementAndClamp:
					o = VK_STENCIL_OP_DECREMENT_AND_CLAMP;
					break;
				case StencilOperator::Invert:
					o = VK_STENCIL_OP_INVERT;
					break;
				case StencilOperator::IncrementAndWrap:
					o = VK_STENCIL_OP_INCREMENT_AND_WRAP;
					break;
				case StencilOperator::DecrementAndWrap:
					o = VK_STENCIL_OP_DECREMENT_AND_WRAP;
					break;

				default:
					ASSERTION_FAILED("Unknown StencilOperator ({0})", (int)op);
					break;
			}

			return o;
		}

		VkBlendFactor GetVkBlendFactor(BlendFactor blendFactor)
		{
			VkBlendFactor f;

			switch(blendFactor) {
				case BlendFactor::Zero:
					f = VK_BLEND_FACTOR_ZERO;
					break;
				case BlendFactor::One:
					f = VK_BLEND_FACTOR_ONE;
					break;
				case BlendFactor::SourceColor:
					f = VK_BLEND_FACTOR_SRC_COLOR;
					break;
				case BlendFactor::OneMinusSourceColor:
					f = VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
					break;
				case BlendFactor::DestinationColor:
					f = VK_BLEND_FACTOR_DST_COLOR;
					break;
				case BlendFactor::OneMinusDestinationColor:
					f = VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
					break;
				case BlendFactor::SourceAlpha:
					f = VK_BLEND_FACTOR_SRC_ALPHA;
					break;
				case BlendFactor::OneMinusSourceAlpha:
					f = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
					break;
				case BlendFactor::DestinationAlpha:
					f = VK_BLEND_FACTOR_DST_ALPHA;
					break;
				case BlendFactor::OneMinusDestinationAlpha:
					f = VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
					break;
				case BlendFactor::ConstantColor:
					f = VK_BLEND_FACTOR_CONSTANT_COLOR;
					break;
				case BlendFactor::OneMinusConstantColor:
					f = VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR;
					break;
				case BlendFactor::ConstantAlpha:
					f = VK_BLEND_FACTOR_CONSTANT_ALPHA;
					break;
				case BlendFactor::OneMinusConstantAlpha:
					f = VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA;
					break;

				default:
					ASSERTION_FAILED("Unknown BlendFactor ({0})", (int)blendFactor);
					break;
			}

			return f;
		}

		VkBlendOp GetVkBlendOp(BlendOperator blendOperator)
		{
			VkBlendOp o;

			switch(blendOperator) {
				case BlendOperator::Add:
					o = VK_BLEND_OP_ADD;
					break;
				case BlendOperator::Subtract:
					o = VK_BLEND_OP_SUBTRACT;
					break;
				case BlendOperator::ReverseSubtract:
					o = VK_BLEND_OP_REVERSE_SUBTRACT;
					break;
				case BlendOperator::Min:
					o = VK_BLEND_OP_MIN;
					break;
				case BlendOperator::Max:
					o = VK_BLEND_OP_MAX;
					break;

				default:
					ASSERTION_FAILED("Unknown BlendOperator ({0})", (int)blendOperator);
					break;
			}

			return o;
		}

		VkLogicOp GetVkLogicOp(LogicOperator logicOperator)
		{
			VkLogicOp o;

			switch(logicOperator) {
				case LogicOperator::Clear:
					o = VK_LOGIC_OP_CLEAR;
					break;
				case LogicOperator::And:
					o = VK_LOGIC_OP_AND;
					break;
				case LogicOperator::AndReverse:
					o = VK_LOGIC_OP_AND_REVERSE;
					break;
				case LogicOperator::Copy:
					o = VK_LOGIC_OP_COPY;
					break;
				case LogicOperator::AndInverted:
					o = VK_LOGIC_OP_AND_INVERTED;
					break;
				case LogicOperator::NoOp:
					o = VK_LOGIC_OP_NO_OP;
					break;
				case LogicOperator::Xor:
					o = VK_LOGIC_OP_XOR;
					break;
				case LogicOperator::Or:
					o = VK_LOGIC_OP_OR;
					break;
				case LogicOperator::Nor:
					o = VK_LOGIC_OP_NOR;
					break;
				case LogicOperator::Equivalent:
					o = VK_LOGIC_OP_EQUIVALENT;
					break;
				case LogicOperator::Invert:
					o = VK_LOGIC_OP_INVERT;
					break;
				case LogicOperator::OrReverse:
					o = VK_LOGIC_OP_OR_REVERSE;
					break;
				case LogicOperator::CopyInverted:
					o = VK_LOGIC_OP_COPY_INVERTED;
					break;
				case LogicOperator::OrInverted:
					o = VK_LOGIC_OP_OR_INVERTED;
					break;
				case LogicOperator::Nand:
					o = VK_LOGIC_OP_NAND;
					break;
				case LogicOperator::Set:
					o = VK_LOGIC_OP_SET;
					break;

				default:
					ASSERTION_FAILED("Unknown LogicOperator ({0})", (int)logicOperator);
					break;
			}

			return o;
		}

		VkClearColorValue GetVkClearColorValue(Color color)
		{
			VkClearColorValue v;

			memcpy(&v, &color, sizeof(Color));

			return v;
		}

		VkClearDepthStencilValue GetVkClearDepthStencilValue(DepthStencilValue value)
		{
			VkClearDepthStencilValue v;

			v.depth = value.depth;
			v.stencil = value.stencil;

			return v;
		}

		VkAccessFlags GetVkAccessFlags(AccessBits access)
		{
			VkAccessFlags f = 0;

			if(static_cast<int>(access & AccessBits::IndirectCommandReadBit) > 0)
				f |= VK_ACCESS_INDIRECT_COMMAND_READ_BIT;

			if(static_cast<int>(access & AccessBits::IndexReadBit) > 0)
				f |= VK_ACCESS_INDEX_READ_BIT;

			if(static_cast<int>(access & AccessBits::VertexAttributeReadBit) > 0)
				f |= VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT;

			if(static_cast<int>(access & AccessBits::UniformReadBit) > 0)
				f |= VK_ACCESS_UNIFORM_READ_BIT;

			if(static_cast<int>(access & AccessBits::InputAttachmentReadBit) > 0)
				f |= VK_ACCESS_INPUT_ATTACHMENT_READ_BIT;

			if(static_cast<int>(access & AccessBits::ShaderReadBit) > 0)
				f |= VK_ACCESS_SHADER_READ_BIT;

			if(static_cast<int>(access & AccessBits::ShaderWriteBit) > 0)
				f |= VK_ACCESS_SHADER_WRITE_BIT;

			if(static_cast<int>(access & AccessBits::ColorAttachmentReadBit) > 0)
				f |= VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;

			if(static_cast<int>(access & AccessBits::ColorAttachmentWriteBit) > 0)
				f |= VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

			if(static_cast<int>(access & AccessBits::DepthStencilAttachmentReadBit) > 0)
				f |= VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;

			if(static_cast<int>(access & AccessBits::DepthStencilAttachmentWriteBit) > 0)
				f |= VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

			if(static_cast<int>(access & AccessBits::TransferReadBit) > 0)
				f |= VK_ACCESS_TRANSFER_READ_BIT;

			if(static_cast<int>(access & AccessBits::TransferWriteBit) > 0)
				f |= VK_ACCESS_TRANSFER_WRITE_BIT;

			if(static_cast<int>(access & AccessBits::HostReadBit) > 0)
				f |= VK_ACCESS_HOST_READ_BIT;

			if(static_cast<int>(access & AccessBits::HostWriteBit) > 0)
				f |= VK_ACCESS_HOST_WRITE_BIT;

			if(static_cast<int>(access & AccessBits::MemoryReadBit) > 0)
				f |= VK_ACCESS_MEMORY_READ_BIT;

			if(static_cast<int>(access & AccessBits::MemoryWriteBit) > 0)
				f |= VK_ACCESS_MEMORY_WRITE_BIT;

			return f;
		}

		VkShaderStageFlags GetVkShaderStageFlags(ShaderTypeBits shaderType)
		{
			VkShaderStageFlags shaderStageFlags = 0;

			if(static_cast<int>(shaderType & ShaderTypeBits::Vertex) > 0)
				shaderStageFlags |= VK_SHADER_STAGE_VERTEX_BIT;

			if(static_cast<int>(shaderType & ShaderTypeBits::Pixel) > 0)
				shaderStageFlags |= VK_SHADER_STAGE_FRAGMENT_BIT;

			if(static_cast<int>(shaderType & ShaderTypeBits::Fragment) > 0)
				shaderStageFlags |= VK_SHADER_STAGE_FRAGMENT_BIT;

			if(static_cast<int>(shaderType & ShaderTypeBits::Geometry) > 0)
				shaderStageFlags |= VK_SHADER_STAGE_GEOMETRY_BIT;

			if(static_cast<int>(shaderType & ShaderTypeBits::TessellationControl) > 0)
				shaderStageFlags |= VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;

			if(static_cast<int>(shaderType & ShaderTypeBits::TessellationEvaluation) > 0)
				shaderStageFlags |= VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;

			if(static_cast<int>(shaderType & ShaderTypeBits::Compute) > 0)
				shaderStageFlags |= VK_SHADER_STAGE_COMPUTE_BIT;

			return shaderStageFlags;
		}
	} // namespace render
} // namespace cube
