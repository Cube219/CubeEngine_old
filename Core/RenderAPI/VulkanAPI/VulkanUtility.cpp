#include "VulkanUtility.h"

#include <memory.h>

#ifdef _DEBUG
const wchar_t* _CheckVkResult(const wchar_t* sayer, const wchar_t* msg, VkResult res)
{
	if(res != VK_SUCCESS) {
		wchar_t* errMsg;
		switch(res) {
			case VK_ERROR_OUT_OF_HOST_MEMORY:
				errMsg = L"(VK_ERROR_OUT_OF_HOST_MEMORY)";
				break;
			case VK_ERROR_OUT_OF_DEVICE_MEMORY:
				errMsg = L"(VK_ERROR_OUT_OF_DEVICE_MEMORY)";
				break;
			case VK_ERROR_INITIALIZATION_FAILED:
				errMsg = L"(VK_ERROR_INITIALIZATION_FAILED)";
				break;
			case VK_ERROR_DEVICE_LOST:
				errMsg = L"(VK_ERROR_DEVICE_LOST)";
				break;
			case VK_ERROR_MEMORY_MAP_FAILED:
				errMsg = L"(VK_ERROR_MEMORY_MAP_FAILED)";
				break;
			case VK_ERROR_LAYER_NOT_PRESENT:
				errMsg = L"(VK_ERROR_LAYER_NOT_PRESENT)";
				break;
			case VK_ERROR_EXTENSION_NOT_PRESENT:
				errMsg = L"(VK_ERROR_EXTENSION_NOT_PRESENT)";
				break;
			case VK_ERROR_FEATURE_NOT_PRESENT:
				errMsg = L"(VK_ERROR_FEATURE_NOT_PRESENT)";
				break;
			case VK_ERROR_INCOMPATIBLE_DRIVER:
				errMsg = L"Cannot find a compatible Vulkan ICD (VK_ERROR_INCOMPATIBLE_DRIVER)";
				break;
			case VK_ERROR_TOO_MANY_OBJECTS:
				errMsg = L"(VK_ERROR_TOO_MANY_OBJECTS)";
				break;
			case VK_ERROR_FORMAT_NOT_SUPPORTED:
				errMsg = L"(VK_ERROR_FORMAT_NOT_SUPPORTED)";
				break;
			case VK_ERROR_FRAGMENTED_POOL:
				errMsg = L"(VK_ERROR_FRAGMENTED_POOL)";
				break;
			case VK_ERROR_SURFACE_LOST_KHR:
				errMsg = L"(VK_ERROR_SURFACE_LOST_KHR)";
				break;
			case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
				errMsg = L"(VK_ERROR_NATIVE_WINDOW_IN_USE_KHR)";
				break;
			case VK_ERROR_OUT_OF_DATE_KHR:
				errMsg = L"(VK_ERROR_OUT_OF_DATE_KHR)";
				break;
			case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
				errMsg = L"(VK_ERROR_INCOMPATIBLE_DISPLAY_KHR)";
				break;
			case VK_ERROR_VALIDATION_FAILED_EXT:
				errMsg = L"(VK_ERROR_VALIDATION_FAILED_EXT)";
				break;
			case VK_ERROR_INVALID_SHADER_NV:
				errMsg = L"(VK_ERROR_INVALID_SHADER_NV)";
				break;
			case VK_ERROR_OUT_OF_POOL_MEMORY_KHR:
				errMsg = L"(VK_ERROR_OUT_OF_POOL_MEMORY_KHR)";
				break;
			case VK_ERROR_INVALID_EXTERNAL_HANDLE_KHX:
				errMsg = L"(VK_ERROR_INVALID_EXTERNAL_HANDLE_KHX)";
				break;
			default:
				errMsg = L"(UNKNOWN)";
				break;
		}
		PrintLogWithSayer(sayer, "VkResult Failed! - ");
		
		PrintLog(msg);

		PrintLog(" ");
		PrintlnLog(errMsg);

		return errMsg;
	}
	return nullptr;
}
#endif // _DEBUG

namespace cube
{
	namespace core
	{
		VkFormat GetVkFormat(DataFormat format)
		{
			VkFormat f;

			switch(format) {
				case DataFormat::R32G32B32A32_SFloat:
					f = VK_FORMAT_R32G32B32A32_SFLOAT;
					break;
				case DataFormat::D16_Unorm:
					f = VK_FORMAT_D16_UNORM;
					break;

				default:
					PrintLogWithSayer(L"VulkanPipeline", L"Unknown DataFormat");
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
					PrintLogWithSayer(L"VulkanPipeline", L"Unknown VertexTopology");
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
					PrintLogWithSayer(L"VulkanPipeline", L"Unknown PolygonMode");
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
					PrintLogWithSayer(L"VulkanPipeline", L"Unknown PolygonFrontFace");
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
					PrintLogWithSayer(L"VulkanPipeline", L"Unknown CullMode");
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
					PrintLogWithSayer(L"VulkanPipeline", L"Unknown CompareOperator");
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
					PrintLogWithSayer(L"VulkanPipeline", L"Unknown StencilOperator");
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
					PrintLogWithSayer(L"VulkanPipeline", L"Unknown BlendFactor");
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
					PrintLogWithSayer(L"VulkanPipeline", L"Unknown BlendOperator");
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
					PrintLogWithSayer(L"VulkanPipeline", L"Unknown LogicOperator");
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
	}
}