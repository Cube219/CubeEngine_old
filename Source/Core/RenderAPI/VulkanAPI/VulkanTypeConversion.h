#pragma once

#include "VulkanAPIHeader.h"

#include "RenderAPI/Interface/RenderTypes.h"
#include "Interface/GraphicsPipelineStateVk.h"

namespace cube
{
	namespace render
	{
		void TypeConversionInit();

		VkFormat TextureFormatToVkFormat(TextureFormat texFormat);
		VkCompareOp ComparisonFunctionToVkCompareOp(ComparisonFunction compareFunc);
		VkStencilOpState StencilStateToVkStencilOpState(StencilState stencilState);
		VkStencilOp StencilOperatorToVkStencilOp(StencilOperator stencilOp);
		VkBlendFactor BlendFactorToVkBlendFactor(BlendFactor blendFactor);
		VkBlendOp BlendOperatorToVkBlendOp(BlendOperator blendOp);
		VkAttachmentLoadOp LoadOperatorToVkAttachmentLoadOp(LoadOperator loadOp);
		VkAttachmentStoreOp StoreOperatorToVkAttachmentStoreOp(StoreOperator storeOp);
		VkFilter FilterTypeToVkFilter(FilterType filter);
		VkSamplerMipmapMode FilterTypeToVkSamplerMipmapMode(FilterType filter);
		VkSamplerAddressMode AddressModeToVkSamplerAddressMode(AddressMode addressMode);

		static Array<VkFormat, (Uint32)TextureFormat::TextureFormatCount> texFmtToVkFmt;
	} // namespace render
} // namespace cube
