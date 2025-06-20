#include "VulkanTypeConversion.h"

#include "EngineCore/Assertion.h"

namespace cube
{
	namespace render
	{
		void TypeConversionInit()
		{
			texFmtToVkFmt[(Uint32)TextureFormat::Undefined] = VK_FORMAT_UNDEFINED;

			texFmtToVkFmt[(Uint32)TextureFormat::R_8_Uint] = VK_FORMAT_R8_UINT;
			texFmtToVkFmt[(Uint32)TextureFormat::R_8_Sint] = VK_FORMAT_R8_SINT;
			texFmtToVkFmt[(Uint32)TextureFormat::R_8_UNorm] = VK_FORMAT_R8_UNORM;
			texFmtToVkFmt[(Uint32)TextureFormat::R_8_SNorm] = VK_FORMAT_R8_SNORM;
			texFmtToVkFmt[(Uint32)TextureFormat::R_8_sRGB] = VK_FORMAT_R8_SRGB;

			texFmtToVkFmt[(Uint32)TextureFormat::R_16_Float] = VK_FORMAT_R16_SFLOAT;
			texFmtToVkFmt[(Uint32)TextureFormat::R_16_Uint] = VK_FORMAT_R16_UINT;
			texFmtToVkFmt[(Uint32)TextureFormat::R_16_Sint] = VK_FORMAT_R16_SINT;
			texFmtToVkFmt[(Uint32)TextureFormat::R_16_UNorm] = VK_FORMAT_R16_UNORM;
			texFmtToVkFmt[(Uint32)TextureFormat::R_16_SNorm] = VK_FORMAT_R16_SNORM;

			texFmtToVkFmt[(Uint32)TextureFormat::R_32_Float] = VK_FORMAT_R32_SFLOAT;
			texFmtToVkFmt[(Uint32)TextureFormat::R_32_Uint] = VK_FORMAT_R32_UINT;
			texFmtToVkFmt[(Uint32)TextureFormat::R_32_Sint] = VK_FORMAT_R32_SINT;

			texFmtToVkFmt[(Uint32)TextureFormat::R_64_Float] = VK_FORMAT_R64_SFLOAT;
			texFmtToVkFmt[(Uint32)TextureFormat::R_64_Uint] = VK_FORMAT_R64_UINT;
			texFmtToVkFmt[(Uint32)TextureFormat::R_64_Sint] = VK_FORMAT_R64_SINT;

			texFmtToVkFmt[(Uint32)TextureFormat::RG_8_Uint] = VK_FORMAT_R8G8_UINT;
			texFmtToVkFmt[(Uint32)TextureFormat::RG_8_Sint] = VK_FORMAT_R8G8_SINT;
			texFmtToVkFmt[(Uint32)TextureFormat::RG_8_UNorm] = VK_FORMAT_R8G8_UNORM;
			texFmtToVkFmt[(Uint32)TextureFormat::RG_8_SNorm] = VK_FORMAT_R8G8_SNORM;
			texFmtToVkFmt[(Uint32)TextureFormat::RG_8_sRGB] = VK_FORMAT_R8G8_SRGB;

			texFmtToVkFmt[(Uint32)TextureFormat::RG_16_Float] = VK_FORMAT_R16G16_SFLOAT;
			texFmtToVkFmt[(Uint32)TextureFormat::RG_16_Uint] = VK_FORMAT_R16G16_UINT;
			texFmtToVkFmt[(Uint32)TextureFormat::RG_16_Sint] = VK_FORMAT_R16G16_SINT;
			texFmtToVkFmt[(Uint32)TextureFormat::RG_16_UNorm] = VK_FORMAT_R16G16_UNORM;
			texFmtToVkFmt[(Uint32)TextureFormat::RG_16_SNorm] = VK_FORMAT_R16G16_SNORM;

			texFmtToVkFmt[(Uint32)TextureFormat::RG_32_Float] = VK_FORMAT_R32G32_SFLOAT;
			texFmtToVkFmt[(Uint32)TextureFormat::RG_32_Uint] = VK_FORMAT_R32G32_UINT;
			texFmtToVkFmt[(Uint32)TextureFormat::RG_32_Sint] = VK_FORMAT_R32G32_SINT;

			texFmtToVkFmt[(Uint32)TextureFormat::RG_64_Float] = VK_FORMAT_R64G64_SFLOAT;
			texFmtToVkFmt[(Uint32)TextureFormat::RG_64_Uint] = VK_FORMAT_R64G64_UINT;
			texFmtToVkFmt[(Uint32)TextureFormat::RG_64_Sint] = VK_FORMAT_R64G64_SINT;

			texFmtToVkFmt[(Uint32)TextureFormat::RGB_8_Uint] = VK_FORMAT_R8G8B8_UINT;
			texFmtToVkFmt[(Uint32)TextureFormat::RGB_8_Sint] = VK_FORMAT_R8G8B8_SINT;
			texFmtToVkFmt[(Uint32)TextureFormat::RGB_8_UNorm] = VK_FORMAT_R8G8B8_UNORM;
			texFmtToVkFmt[(Uint32)TextureFormat::RGB_8_SNorm] = VK_FORMAT_R8G8B8_SNORM;
			texFmtToVkFmt[(Uint32)TextureFormat::RGB_8_sRGB] = VK_FORMAT_R8G8B8_SRGB;

			texFmtToVkFmt[(Uint32)TextureFormat::RGB_16_Float] = VK_FORMAT_R16G16B16_SFLOAT;
			texFmtToVkFmt[(Uint32)TextureFormat::RGB_16_Uint] = VK_FORMAT_R16G16B16_UINT;
			texFmtToVkFmt[(Uint32)TextureFormat::RGB_16_Sint] = VK_FORMAT_R16G16B16_SINT;
			texFmtToVkFmt[(Uint32)TextureFormat::RGB_16_UNorm] = VK_FORMAT_R16G16B16_UNORM;
			texFmtToVkFmt[(Uint32)TextureFormat::RGB_16_SNorm] = VK_FORMAT_R16G16B16_SNORM;

			texFmtToVkFmt[(Uint32)TextureFormat::RGB_32_Float] = VK_FORMAT_R32G32B32_SFLOAT;
			texFmtToVkFmt[(Uint32)TextureFormat::RGB_32_Uint] = VK_FORMAT_R32G32B32_UINT;
			texFmtToVkFmt[(Uint32)TextureFormat::RGB_32_Sint] = VK_FORMAT_R32G32B32_SINT;

			texFmtToVkFmt[(Uint32)TextureFormat::RGB_64_Float] = VK_FORMAT_R64G64B64_SFLOAT;
			texFmtToVkFmt[(Uint32)TextureFormat::RGB_64_Uint] = VK_FORMAT_R64G64B64_UINT;
			texFmtToVkFmt[(Uint32)TextureFormat::RGB_64_Sint] = VK_FORMAT_R64G64B64_SINT;

			texFmtToVkFmt[(Uint32)TextureFormat::BGR_8_Uint] = VK_FORMAT_B8G8R8_UINT;
			texFmtToVkFmt[(Uint32)TextureFormat::BGR_8_Sint] = VK_FORMAT_B8G8R8_SINT;
			texFmtToVkFmt[(Uint32)TextureFormat::BGR_8_UNorm] = VK_FORMAT_B8G8R8_UNORM;
			texFmtToVkFmt[(Uint32)TextureFormat::BGR_8_SNorm] = VK_FORMAT_B8G8R8_SNORM;
			texFmtToVkFmt[(Uint32)TextureFormat::BGR_8_sRGB] = VK_FORMAT_B8G8R8_SRGB;

			texFmtToVkFmt[(Uint32)TextureFormat::RGBA_8_Uint] = VK_FORMAT_R8G8B8A8_UINT;
			texFmtToVkFmt[(Uint32)TextureFormat::RGBA_8_Sint] = VK_FORMAT_R8G8B8A8_SINT;
			texFmtToVkFmt[(Uint32)TextureFormat::RGBA_8_UNorm] = VK_FORMAT_R8G8B8A8_UNORM;
			texFmtToVkFmt[(Uint32)TextureFormat::RGBA_8_SNorm] = VK_FORMAT_R8G8B8A8_SNORM;
			texFmtToVkFmt[(Uint32)TextureFormat::RGBA_8_sRGB] = VK_FORMAT_R8G8B8A8_SRGB;

			texFmtToVkFmt[(Uint32)TextureFormat::RGBA_16_Float] = VK_FORMAT_R16G16B16A16_SFLOAT;
			texFmtToVkFmt[(Uint32)TextureFormat::RGBA_16_Uint] = VK_FORMAT_R16G16B16A16_UINT;
			texFmtToVkFmt[(Uint32)TextureFormat::RGBA_16_Sint] = VK_FORMAT_R16G16B16A16_SINT;
			texFmtToVkFmt[(Uint32)TextureFormat::RGBA_16_UNorm] = VK_FORMAT_R16G16B16A16_UNORM;
			texFmtToVkFmt[(Uint32)TextureFormat::RGBA_16_SNorm] = VK_FORMAT_R16G16B16A16_SNORM;

			texFmtToVkFmt[(Uint32)TextureFormat::RGBA_32_Float] = VK_FORMAT_R32G32B32A32_SFLOAT;
			texFmtToVkFmt[(Uint32)TextureFormat::RGBA_32_Uint] = VK_FORMAT_R32G32B32A32_UINT;
			texFmtToVkFmt[(Uint32)TextureFormat::RGBA_32_Sint] = VK_FORMAT_R32G32B32A32_SINT;

			texFmtToVkFmt[(Uint32)TextureFormat::RGBA_64_Float] = VK_FORMAT_R64G64B64A64_SFLOAT;
			texFmtToVkFmt[(Uint32)TextureFormat::RGBA_64_Uint] = VK_FORMAT_R64G64B64A64_UINT;
			texFmtToVkFmt[(Uint32)TextureFormat::RGBA_64_Sint] = VK_FORMAT_R64G64B64A64_SINT;

			texFmtToVkFmt[(Uint32)TextureFormat::BGRA_8_Uint] = VK_FORMAT_B8G8R8A8_UINT;
			texFmtToVkFmt[(Uint32)TextureFormat::BGRA_8_Sint] = VK_FORMAT_B8G8R8A8_SINT;
			texFmtToVkFmt[(Uint32)TextureFormat::BGRA_8_UNorm] = VK_FORMAT_B8G8R8A8_UNORM;
			texFmtToVkFmt[(Uint32)TextureFormat::BGRA_8_SNorm] = VK_FORMAT_B8G8R8A8_SNORM;
			texFmtToVkFmt[(Uint32)TextureFormat::BGRA_8_sRGB] = VK_FORMAT_B8G8R8A8_SRGB;

			// ABGR_8 // 써도 되나 확인

			// A2RGB_10 // 써도 되나 확인

			// A2BGR_10 // 써도 되나 확인

			texFmtToVkFmt[(Uint32)TextureFormat::D16_UNorm] = VK_FORMAT_D16_UNORM;

			texFmtToVkFmt[(Uint32)TextureFormat::D32_Float] = VK_FORMAT_D32_SFLOAT;

			texFmtToVkFmt[(Uint32)TextureFormat::BC1_RGB_UNorm] = VK_FORMAT_BC1_RGB_UNORM_BLOCK;
			texFmtToVkFmt[(Uint32)TextureFormat::BC1_RGB_sRGB] = VK_FORMAT_BC1_RGB_SRGB_BLOCK;
			texFmtToVkFmt[(Uint32)TextureFormat::BC1_RGBA_UNorm] = VK_FORMAT_BC1_RGBA_UNORM_BLOCK;
			texFmtToVkFmt[(Uint32)TextureFormat::BC1_RGBA_sRGB] = VK_FORMAT_BC1_RGBA_SRGB_BLOCK;

			texFmtToVkFmt[(Uint32)TextureFormat::BC2_UNorm] = VK_FORMAT_BC2_UNORM_BLOCK;
			texFmtToVkFmt[(Uint32)TextureFormat::BC2_sRGB] = VK_FORMAT_BC2_SRGB_BLOCK;

			texFmtToVkFmt[(Uint32)TextureFormat::BC3_UNorm] = VK_FORMAT_BC3_UNORM_BLOCK;
			texFmtToVkFmt[(Uint32)TextureFormat::BC3_sRGB] = VK_FORMAT_BC3_SRGB_BLOCK;

			texFmtToVkFmt[(Uint32)TextureFormat::BC4_UNorm] = VK_FORMAT_BC4_UNORM_BLOCK;
			texFmtToVkFmt[(Uint32)TextureFormat::BC4_SNorm] = VK_FORMAT_BC4_SNORM_BLOCK;

			texFmtToVkFmt[(Uint32)TextureFormat::BC5_UNorm] = VK_FORMAT_BC5_UNORM_BLOCK;
			texFmtToVkFmt[(Uint32)TextureFormat::BC5_SNorm] = VK_FORMAT_BC5_SNORM_BLOCK;

			texFmtToVkFmt[(Uint32)TextureFormat::BC6_UFloat] = VK_FORMAT_BC6H_UFLOAT_BLOCK;
			texFmtToVkFmt[(Uint32)TextureFormat::BC6_SFloat] = VK_FORMAT_BC6H_SFLOAT_BLOCK;

			texFmtToVkFmt[(Uint32)TextureFormat::BC7_UNorm] = VK_FORMAT_BC7_UNORM_BLOCK;
			texFmtToVkFmt[(Uint32)TextureFormat::BC7_sRGB] = VK_FORMAT_BC7_SRGB_BLOCK;

			texFmtToVkFmt[(Uint32)TextureFormat::ETC2_RGB_8_UNorm] = VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK;
			texFmtToVkFmt[(Uint32)TextureFormat::ETC2_RGB_8_sRGB] = VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK;

			texFmtToVkFmt[(Uint32)TextureFormat::ETC2_RGBA1_8_UNorm] = VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK;
			texFmtToVkFmt[(Uint32)TextureFormat::ETC2_RGBA1_8_sRGB] = VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK;

			texFmtToVkFmt[(Uint32)TextureFormat::ETC2_RGBA_8_UNorm] = VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK;
			texFmtToVkFmt[(Uint32)TextureFormat::ETC2_RGBA_8_sRGB] = VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK;

			texFmtToVkFmt[(Uint32)TextureFormat::EAC_R_11_UNorm] = VK_FORMAT_EAC_R11_UNORM_BLOCK;
			texFmtToVkFmt[(Uint32)TextureFormat::EAC_R_11_SNorm] = VK_FORMAT_EAC_R11_SNORM_BLOCK;

			texFmtToVkFmt[(Uint32)TextureFormat::EAC_RG_11_UNorm] = VK_FORMAT_EAC_R11G11_UNORM_BLOCK;
			texFmtToVkFmt[(Uint32)TextureFormat::EAC_RG_11_SNorm] = VK_FORMAT_EAC_R11G11_SNORM_BLOCK;

			texFmtToVkFmt[(Uint32)TextureFormat::ASTC_4x4_UNorm] = VK_FORMAT_ASTC_4x4_UNORM_BLOCK;
			texFmtToVkFmt[(Uint32)TextureFormat::ASTC_4x4_sRGB] = VK_FORMAT_ASTC_4x4_SRGB_BLOCK;

			texFmtToVkFmt[(Uint32)TextureFormat::ASTC_5x4_UNorm] = VK_FORMAT_ASTC_5x4_UNORM_BLOCK;
			texFmtToVkFmt[(Uint32)TextureFormat::ASTC_5x4_sRGB] = VK_FORMAT_ASTC_5x4_SRGB_BLOCK;

			texFmtToVkFmt[(Uint32)TextureFormat::ASTC_5x5_UNorm] = VK_FORMAT_ASTC_5x5_UNORM_BLOCK;
			texFmtToVkFmt[(Uint32)TextureFormat::ASTC_5x5_sRGB] = VK_FORMAT_ASTC_5x5_SRGB_BLOCK;

			texFmtToVkFmt[(Uint32)TextureFormat::ASTC_6x5_UNorm] = VK_FORMAT_ASTC_6x5_UNORM_BLOCK;
			texFmtToVkFmt[(Uint32)TextureFormat::ASTC_6x5_sRGB] = VK_FORMAT_ASTC_6x5_SRGB_BLOCK;

			texFmtToVkFmt[(Uint32)TextureFormat::ASTC_6x6_UNorm] = VK_FORMAT_ASTC_6x6_UNORM_BLOCK;
			texFmtToVkFmt[(Uint32)TextureFormat::ASTC_6x6_sRGB] = VK_FORMAT_ASTC_6x6_SRGB_BLOCK;

			texFmtToVkFmt[(Uint32)TextureFormat::ASTC_8x5_UNorm] = VK_FORMAT_ASTC_8x5_UNORM_BLOCK;
			texFmtToVkFmt[(Uint32)TextureFormat::ASTC_8x5_sRGB] = VK_FORMAT_ASTC_8x5_SRGB_BLOCK;

			texFmtToVkFmt[(Uint32)TextureFormat::ASTC_8x6_UNorm] = VK_FORMAT_ASTC_8x6_UNORM_BLOCK;
			texFmtToVkFmt[(Uint32)TextureFormat::ASTC_8x6_sRGB] = VK_FORMAT_ASTC_8x6_SRGB_BLOCK;

			texFmtToVkFmt[(Uint32)TextureFormat::ASTC_8x8_UNorm] = VK_FORMAT_ASTC_8x8_UNORM_BLOCK;
			texFmtToVkFmt[(Uint32)TextureFormat::ASTC_8x8_sRGB] = VK_FORMAT_ASTC_8x8_SRGB_BLOCK;

			texFmtToVkFmt[(Uint32)TextureFormat::ASTC_10x5_UNorm] = VK_FORMAT_ASTC_10x5_UNORM_BLOCK;
			texFmtToVkFmt[(Uint32)TextureFormat::ASTC_10x5_sRGB] = VK_FORMAT_ASTC_10x5_SRGB_BLOCK;

			texFmtToVkFmt[(Uint32)TextureFormat::ASTC_10x6_UNorm] = VK_FORMAT_ASTC_10x6_UNORM_BLOCK;
			texFmtToVkFmt[(Uint32)TextureFormat::ASTC_10x6_sRGB] = VK_FORMAT_ASTC_10x6_SRGB_BLOCK;

			texFmtToVkFmt[(Uint32)TextureFormat::ASTC_10x8_UNorm] = VK_FORMAT_ASTC_10x8_UNORM_BLOCK;
			texFmtToVkFmt[(Uint32)TextureFormat::ASTC_10x8_sRGB] = VK_FORMAT_ASTC_10x8_SRGB_BLOCK;

			texFmtToVkFmt[(Uint32)TextureFormat::ASTC_10x10_UNorm] = VK_FORMAT_ASTC_10x10_UNORM_BLOCK;
			texFmtToVkFmt[(Uint32)TextureFormat::ASTC_10x10_sRGB] = VK_FORMAT_ASTC_10x10_SRGB_BLOCK;

			texFmtToVkFmt[(Uint32)TextureFormat::ASTC_12x10_UNorm] = VK_FORMAT_ASTC_12x10_UNORM_BLOCK;
			texFmtToVkFmt[(Uint32)TextureFormat::ASTC_12x10_sRGB] = VK_FORMAT_ASTC_12x10_SRGB_BLOCK;

			texFmtToVkFmt[(Uint32)TextureFormat::ASTC_12x12_UNorm] = VK_FORMAT_ASTC_12x12_UNORM_BLOCK;
			texFmtToVkFmt[(Uint32)TextureFormat::ASTC_12x12_sRGB] = VK_FORMAT_ASTC_12x12_SRGB_BLOCK;
		}

		VkFormat TextureFormatToVkFormat(TextureFormat texFormat)
		{
			return texFmtToVkFmt[(Uint32)texFormat];
		}

		VkCompareOp ComparisonFunctionToVkCompareOp(ComparisonFunction compareFunc)
		{
			switch(compareFunc) {
				case ComparisonFunction::Never:        return VK_COMPARE_OP_NEVER;
				case ComparisonFunction::Always:       return VK_COMPARE_OP_ALWAYS;
				case ComparisonFunction::Equal:        return VK_COMPARE_OP_EQUAL;
				case ComparisonFunction::NotEqual:     return VK_COMPARE_OP_NOT_EQUAL;
				case ComparisonFunction::Less:         return VK_COMPARE_OP_LESS;
				case ComparisonFunction::LessEqual:    return VK_COMPARE_OP_LESS_OR_EQUAL;
				case ComparisonFunction::Greater:      return VK_COMPARE_OP_GREATER;
				case ComparisonFunction::GreaterEqual: return VK_COMPARE_OP_GREATER_OR_EQUAL;
				default: ASSERTION_FAILED("Unknown ComparisonFunction ({0})", (Uint32)compareFunc);
			}
			return VK_COMPARE_OP_NEVER;
		}

		VkStencilOpState StencilStateToVkStencilOpState(StencilState stencilState)
		{
			VkStencilOpState state;
			state.depthFailOp = StencilOperatorToVkStencilOp(stencilState.depthFailOp);
			state.failOp = StencilOperatorToVkStencilOp(stencilState.failOp);
			state.passOp = StencilOperatorToVkStencilOp(stencilState.passOp);
			state.compareOp = ComparisonFunctionToVkCompareOp(stencilState.compareFunc);
			state.compareMask = stencilState.readMask;
			state.writeMask = stencilState.writeMask;
			state.reference = 0; // Set dynamically

			return state;
		}

		VkStencilOp StencilOperatorToVkStencilOp(StencilOperator stencilOp)
		{
			switch(stencilOp) {
				case StencilOperator::Zero:              return VK_STENCIL_OP_ZERO;
				case StencilOperator::Keep:              return VK_STENCIL_OP_KEEP;
				case StencilOperator::Invert:            return VK_STENCIL_OP_INVERT;
				case StencilOperator::Replace:           return VK_STENCIL_OP_REPLACE;
				case StencilOperator::IncrementAndClamp: return VK_STENCIL_OP_INCREMENT_AND_CLAMP;
				case StencilOperator::IncrementAndWrap:  return VK_STENCIL_OP_INCREMENT_AND_WRAP;
				case StencilOperator::DecrementAndClamp: return VK_STENCIL_OP_DECREMENT_AND_CLAMP;
				case StencilOperator::DecrementAndWrap:  return VK_STENCIL_OP_DECREMENT_AND_WRAP;
				default: ASSERTION_FAILED("Unknown StencilOperator ({0})", (Uint32)stencilOp);
			}
			return VK_STENCIL_OP_ZERO;
		}

		VkBlendFactor BlendFactorToVkBlendFactor(BlendFactor blendFactor)
		{
			switch(blendFactor) {
				case BlendFactor::Zero:                    return VK_BLEND_FACTOR_ZERO;
				case BlendFactor::One:                     return VK_BLEND_FACTOR_ONE;
				case BlendFactor::SourceColor:             return VK_BLEND_FACTOR_SRC_COLOR;
				case BlendFactor::SourceAlpha:             return VK_BLEND_FACTOR_SRC_ALPHA;
				case BlendFactor::SourceAlphaSaturate:     return VK_BLEND_FACTOR_SRC_ALPHA_SATURATE;
				case BlendFactor::InverseSourceColor:      return VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
				case BlendFactor::InverseSourceAlpha:      return VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
				case BlendFactor::DestinationColor:        return VK_BLEND_FACTOR_DST_COLOR;
				case BlendFactor::DestinationAlpha:        return VK_BLEND_FACTOR_DST_ALPHA;
				case BlendFactor::InverseDestinationColor: return VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
				case BlendFactor::InverseDestinationAlpha: return VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
				case BlendFactor::Constant:                return VK_BLEND_FACTOR_CONSTANT_COLOR;
				case BlendFactor::InverseConstant:         return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR;
				// TODO: CONSTANT_ALPHA는 나중에?
				case BlendFactor::Source1Color:            return VK_BLEND_FACTOR_SRC1_COLOR;
				case BlendFactor::Source1Alpha:            return VK_BLEND_FACTOR_SRC1_ALPHA;
				case BlendFactor::InverseSource1Color:     return VK_BLEND_FACTOR_ONE_MINUS_SRC1_COLOR;
				case BlendFactor::InverseSource1Alpha:     return VK_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA;
				default: ASSERTION_FAILED("Unknown BlendFactor ({0})", (Uint32)blendFactor);
			}
			return VK_BLEND_FACTOR_ZERO;
		}

		VkBlendOp BlendOperatorToVkBlendOp(BlendOperator blendOp)
		{
			switch(blendOp) {
				case BlendOperator::Add:             return VK_BLEND_OP_ADD;
				case BlendOperator::Subtract:        return VK_BLEND_OP_SUBTRACT;
				case BlendOperator::ReverseSubtract: return VK_BLEND_OP_REVERSE_SUBTRACT;
				case BlendOperator::Min:             return VK_BLEND_OP_MIN;
				case BlendOperator::Max:             return VK_BLEND_OP_MAX;
				default: ASSERTION_FAILED("Unknown BlendOperator ({0})", (Uint32)blendOp);
			}
			return VK_BLEND_OP_ADD;
		}

		VkAttachmentLoadOp LoadOperatorToVkAttachmentLoadOp(LoadOperator loadOp)
		{
			switch(loadOp) {
				case LoadOperator::Load:     return VK_ATTACHMENT_LOAD_OP_LOAD;
				case LoadOperator::Clear:    return VK_ATTACHMENT_LOAD_OP_CLEAR;
				case LoadOperator::DontCare: return VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				default: ASSERTION_FAILED("Unknown LoadOperator ({0})", (Uint32)loadOp);
			}
			return VK_ATTACHMENT_LOAD_OP_LOAD;
		}

		VkAttachmentStoreOp StoreOperatorToVkAttachmentStoreOp(StoreOperator storeOp)
		{
			switch(storeOp) {
				case StoreOperator::Store:    return VK_ATTACHMENT_STORE_OP_STORE;
				case StoreOperator::DontCare: return VK_ATTACHMENT_STORE_OP_DONT_CARE;
				default: ASSERTION_FAILED("Unknown StoreOperator ({0})", (Uint32)storeOp);
			}
			return VK_ATTACHMENT_STORE_OP_STORE;
		}

		VkFilter FilterTypeToVkFilter(FilterType filter)
		{
			switch(filter) {
				case FilterType::Point:
					return VK_FILTER_NEAREST;

				case FilterType::Linear:
				case FilterType::Anisotropy:
					return VK_FILTER_LINEAR;

				default:
					ASSERTION_FAILED("Unknown FilterType ({0})", (Uint32)filter);
			}
			return VK_FILTER_NEAREST;
		}

		VkSamplerMipmapMode FilterTypeToVkSamplerMipmapMode(FilterType filter)
		{
			switch(filter) {
				case FilterType::Point:
					return VK_SAMPLER_MIPMAP_MODE_NEAREST;

				case FilterType::Linear:
				case FilterType::Anisotropy:
					return VK_SAMPLER_MIPMAP_MODE_LINEAR;

				default:
					ASSERTION_FAILED("Unknown FilterType ({0})", (Uint32)filter);
			}
			return VK_SAMPLER_MIPMAP_MODE_LINEAR;
		}

		VkSamplerAddressMode AddressModeToVkSamplerAddressMode(AddressMode addressMode)
		{
			switch(addressMode) {
				case AddressMode::Wrap:        return VK_SAMPLER_ADDRESS_MODE_REPEAT;
				case AddressMode::BorderColor: return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
				case AddressMode::Clamp:       return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
				case AddressMode::Mirror:      return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
				case AddressMode::MirrorOnce:  return VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE;
				default: ASSERTION_FAILED("Unknown AddressMode ({0})", (Uint32)addressMode);
			}
			return VK_SAMPLER_ADDRESS_MODE_REPEAT;
		}
	} // namespace render
} // namespace cube
