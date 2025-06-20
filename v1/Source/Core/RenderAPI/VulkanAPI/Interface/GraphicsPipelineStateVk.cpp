#include "GraphicsPipelineStateVk.h"

#include "../VulkanUtility.h"
#include "../VulkanTypeConversion.h"
#include "DeviceVk.h"
#include "ShaderVk.h"
#include "RenderPassVk.h"
#include "ShaderParametersLayoutVk.h"
#include "EngineCore/Assertion.h"

namespace cube
{
	namespace render
	{
		GraphicsPipelineStateVk::GraphicsPipelineStateVk(DeviceVk& device, const GraphicsPipelineStateAttribute& attr) :
			GraphicsPipelineState(attr)
		{
			// Vertex input (InputLayout)
			// layout -> binding
			// element -> attribute
			VkPipelineVertexInputStateCreateInfo vertexInputCreateInfo;
			
			Uint64 layoutCount = attr.inputLayouts.size();
			Uint64 totalElementCount = 0;
			for(Uint64 i = 0; i < layoutCount; i++) {
				totalElementCount += attr.inputLayouts[i].elements.size();
			}

			Vector<VkVertexInputBindingDescription> vertexInputBindingDescs(layoutCount);
			Vector<VkVertexInputAttributeDescription> vertexInputAttrDescs(totalElementCount);

			Uint32 elementIndex = 0;
			for(Uint32 i = 0; i < SCast(Uint32)(layoutCount); i++) {
				auto& inputLayout = attr.inputLayouts[i];

				vertexInputBindingDescs[i].binding = i;
				vertexInputBindingDescs[i].stride = inputLayout.size;
				vertexInputBindingDescs[i].inputRate = VK_VERTEX_INPUT_RATE_VERTEX; // TODO: 인스턴싱때 변경 가능

				for(Uint32 j = 0; j < SCast(Uint32)(inputLayout.elements.size()); j++) {
					auto& element = inputLayout.elements[j];

					vertexInputAttrDescs[elementIndex].binding = i;
					vertexInputAttrDescs[elementIndex].location = j;
					vertexInputAttrDescs[elementIndex].format = TextureFormatToVkFormat(element.format);
					vertexInputAttrDescs[elementIndex].offset = element.offset;

					elementIndex++;
				}
			}

			vertexInputCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
			vertexInputCreateInfo.pNext = nullptr;
			vertexInputCreateInfo.flags = 0;
			vertexInputCreateInfo.vertexBindingDescriptionCount = SCast(Uint32)(vertexInputBindingDescs.size());
			vertexInputCreateInfo.pVertexBindingDescriptions = vertexInputBindingDescs.data();
			vertexInputCreateInfo.vertexAttributeDescriptionCount = SCast(Uint32)(vertexInputAttrDescs.size());
			vertexInputCreateInfo.pVertexAttributeDescriptions = vertexInputAttrDescs.data();

			// Input assembly (PrimitiveTopology)
			VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo;
			inputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
			inputAssemblyStateCreateInfo.pNext = nullptr;
			inputAssemblyStateCreateInfo.flags = 0;
			switch(attr.primitiveTopology) {
				case PrimitiveTopology::PointList:     inputAssemblyStateCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_POINT_LIST; break;
				case PrimitiveTopology::LineList:      inputAssemblyStateCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_LINE_LIST; break;
				case PrimitiveTopology::LineStrip:     inputAssemblyStateCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_LINE_STRIP; break;
				case PrimitiveTopology::TriangleList:  inputAssemblyStateCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST; break;
				case PrimitiveTopology::TriangleStrip: inputAssemblyStateCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP; break;
				case PrimitiveTopology::TriangleFan:   inputAssemblyStateCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN; break;
				case PrimitiveTopology::PatchList:     inputAssemblyStateCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_PATCH_LIST; break;
				default: ASSERTION_FAILED("Unknown PrimitiveTopology ({0})", (Uint32)attr.primitiveTopology);
			}
			inputAssemblyStateCreateInfo.primitiveRestartEnable = VK_FALSE;

			// Viewport / Scissor
			// TODO: 이거 어찌할까? 일단 무조건 dynamic으로?
			VkPipelineViewportStateCreateInfo viewportStateCreateInfo;
			viewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
			viewportStateCreateInfo.pNext = nullptr;
			viewportStateCreateInfo.flags = 0;
			viewportStateCreateInfo.viewportCount = 1;
			viewportStateCreateInfo.pViewports = nullptr; // Use dynamic viewports
			viewportStateCreateInfo.scissorCount = 1;
			viewportStateCreateInfo.pScissors = nullptr; // Use dynamic scissors

			// Rasterization (RasterizerState)
			VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo;
			rasterizationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
			rasterizationStateCreateInfo.pNext = nullptr;
			rasterizationStateCreateInfo.flags = 0;
			switch(attr.rasterizerState.fillMode) {
				case FillMode::Point: rasterizationStateCreateInfo.polygonMode = VK_POLYGON_MODE_POINT; break;
				case FillMode::Line:  rasterizationStateCreateInfo.polygonMode = VK_POLYGON_MODE_LINE; break;
				case FillMode::Fill:  rasterizationStateCreateInfo.polygonMode = VK_POLYGON_MODE_FILL; break;
				default: ASSERTION_FAILED("Unknown FillMode ({0})", (Uint32)attr.rasterizerState.fillMode);
			}
			switch(attr.rasterizerState.cullMode) {
				case CullMode::None:  rasterizationStateCreateInfo.cullMode =  VK_CULL_MODE_NONE; break;
				case CullMode::Front: rasterizationStateCreateInfo.cullMode = VK_CULL_MODE_FRONT_BIT; break;
				case CullMode::Back:  rasterizationStateCreateInfo.cullMode =  VK_CULL_MODE_BACK_BIT; break;
				default: ASSERTION_FAILED("Unknown CullMode ({0})", (Uint32)attr.rasterizerState.cullMode);
			}
			switch(attr.rasterizerState.frontFace) {
			case TriangleFrontFace::Clockwise:        rasterizationStateCreateInfo.frontFace = VK_FRONT_FACE_CLOCKWISE; break;
			case TriangleFrontFace::CounterClockwise: rasterizationStateCreateInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE; break;
			default: ASSERTION_FAILED("Unknown TriangleFrontFace ({0})", (Uint32)attr.rasterizerState.frontFace);
			}
			rasterizationStateCreateInfo.depthClampEnable = VK_FALSE; // TODO: Rasterization 기능확장때 구현
			rasterizationStateCreateInfo.rasterizerDiscardEnable = VK_FALSE;
			rasterizationStateCreateInfo.depthBiasEnable = VK_FALSE;
			rasterizationStateCreateInfo.depthBiasConstantFactor = 0;
			rasterizationStateCreateInfo.depthBiasClamp = 0;
			rasterizationStateCreateInfo.depthBiasSlopeFactor = 0;
			rasterizationStateCreateInfo.lineWidth = 1.0f;

			// Multisampler (TODO: 차후 구현)
			VkPipelineMultisampleStateCreateInfo multisampleStateCreateInfo;
			multisampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
			multisampleStateCreateInfo.pNext = nullptr;
			multisampleStateCreateInfo.flags = 0;
			multisampleStateCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
			multisampleStateCreateInfo.sampleShadingEnable = VK_FALSE;
			multisampleStateCreateInfo.minSampleShading = 0.0f;
			multisampleStateCreateInfo.pSampleMask = nullptr;
			multisampleStateCreateInfo.alphaToCoverageEnable = VK_FALSE;
			multisampleStateCreateInfo.alphaToOneEnable = VK_FALSE;

			// Depth Stencil (DepthStencilState)
			VkPipelineDepthStencilStateCreateInfo depthStencilStateCreateInfo;
			depthStencilStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
			depthStencilStateCreateInfo.pNext = nullptr;
			depthStencilStateCreateInfo.flags = 0;
			depthStencilStateCreateInfo.depthTestEnable = attr.depthStencilState.enableDepthTest;
			depthStencilStateCreateInfo.depthWriteEnable = attr.depthStencilState.enableDepthWrite;
			depthStencilStateCreateInfo.depthCompareOp = ComparisonFunctionToVkCompareOp(attr.depthStencilState.depthCompareFunc);
			depthStencilStateCreateInfo.depthBoundsTestEnable = VK_FALSE;
			depthStencilStateCreateInfo.minDepthBounds = 0.0f;
			depthStencilStateCreateInfo.maxDepthBounds = 1.0f;
			depthStencilStateCreateInfo.stencilTestEnable = attr.depthStencilState.enableStencilTest;
			depthStencilStateCreateInfo.back = StencilStateToVkStencilOpState(attr.depthStencilState.backFace);
			depthStencilStateCreateInfo.front = StencilStateToVkStencilOpState(attr.depthStencilState.frontFace);

			// Color blend (BlendState)
			Vector<VkPipelineColorBlendAttachmentState> colorBlendAttachments(attr.blendState.renderTargets.size());
			for(Uint64 i = 0; i < colorBlendAttachments.size(); i++) {
				auto& blendState = attr.blendState.renderTargets[i];
				auto& attachment = colorBlendAttachments[i];

				attachment.blendEnable = blendState.enableBlend;
				attachment.srcColorBlendFactor = BlendFactorToVkBlendFactor(blendState.srcBlend);
				attachment.dstColorBlendFactor = BlendFactorToVkBlendFactor(blendState.dstBlend);
				attachment.colorBlendOp = BlendOperatorToVkBlendOp(blendState.blendOp);
				attachment.srcAlphaBlendFactor = BlendFactorToVkBlendFactor(blendState.srcAlphaBlend);
				attachment.dstAlphaBlendFactor = BlendFactorToVkBlendFactor(blendState.dstAlphaBlend);
				attachment.alphaBlendOp = BlendOperatorToVkBlendOp(blendState.alphaBlendOp);
				attachment.colorWriteMask = 0;
				if(blendState.writeMask.IsSet(ColorWriteMaskFlag::Red))
					attachment.colorWriteMask |= VK_COLOR_COMPONENT_R_BIT;
				if(blendState.writeMask.IsSet(ColorWriteMaskFlag::Green))
					attachment.colorWriteMask |= VK_COLOR_COMPONENT_G_BIT;
				if(blendState.writeMask.IsSet(ColorWriteMaskFlag::Blue))
					attachment.colorWriteMask |= VK_COLOR_COMPONENT_B_BIT;
				if(blendState.writeMask.IsSet(ColorWriteMaskFlag::Alpha))
					attachment.colorWriteMask |= VK_COLOR_COMPONENT_A_BIT;
			}

			VkPipelineColorBlendStateCreateInfo colorBlendStateCreateInfo;
			colorBlendStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
			colorBlendStateCreateInfo.pNext = nullptr;
			colorBlendStateCreateInfo.flags = 0;
			colorBlendStateCreateInfo.logicOpEnable = false;
			colorBlendStateCreateInfo.logicOp = VK_LOGIC_OP_NO_OP;
			colorBlendStateCreateInfo.attachmentCount = SCast(Uint32)(colorBlendAttachments.size());
			colorBlendStateCreateInfo.pAttachments = colorBlendAttachments.data();
			colorBlendStateCreateInfo.blendConstants[0] = 0.0f;
			colorBlendStateCreateInfo.blendConstants[1] = 0.0f;
			colorBlendStateCreateInfo.blendConstants[2] = 0.0f;
			colorBlendStateCreateInfo.blendConstants[3] = 0.0f;

			// Dynamic states
			Vector<VkDynamicState> dynamicState;
			dynamicState.push_back(VK_DYNAMIC_STATE_VIEWPORT);
			dynamicState.push_back(VK_DYNAMIC_STATE_SCISSOR);

			VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo;
			dynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
			dynamicStateCreateInfo.pNext = nullptr;
			dynamicStateCreateInfo.flags = 0;
			dynamicStateCreateInfo.dynamicStateCount = SCast(uint32_t)(dynamicState.size());
			dynamicStateCreateInfo.pDynamicStates = dynamicState.data();

			// Shader stages
			Vector<VkPipelineShaderStageCreateInfo> shaderStages;
			if(attr.vertexShader != nullptr)
				shaderStages.push_back(DPCast(ShaderVk)(attr.vertexShader)->GetPipelineShaderStageInfo());
			if(attr.pixelShader != nullptr)
				shaderStages.push_back(DPCast(ShaderVk)(attr.pixelShader)->GetPipelineShaderStageInfo());
			if(attr.domainShader != nullptr)
				shaderStages.push_back(DPCast(ShaderVk)(attr.domainShader)->GetPipelineShaderStageInfo());
			if(attr.hullShader != nullptr)
				shaderStages.push_back(DPCast(ShaderVk)(attr.hullShader)->GetPipelineShaderStageInfo());
			if(attr.geometryShader != nullptr)
				shaderStages.push_back(DPCast(ShaderVk)(attr.geometryShader)->GetPipelineShaderStageInfo());

			// Pipeline layout (ShaderParametersLayout)
			// Equivalent in ComputePipelineStateVk
			VkPipelineLayoutCreateInfo layoutInfo;
			layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
			layoutInfo.pNext = nullptr;
			layoutInfo.flags = 0;

			Vector<VkDescriptorSetLayout> descSetLayouts(attr.shaderParameterLayouts.size());

			VkPushConstantRange pushConstantRange;
			layoutInfo.pushConstantRangeCount = 0;
			layoutInfo.pPushConstantRanges = nullptr;

			for(Uint64 i = 0; i < attr.shaderParameterLayouts.size(); i++) {
				SPtr<ShaderParametersLayoutVk> layout = DPCast(ShaderParametersLayoutVk)(attr.shaderParameterLayouts[i]);

				if(layout->HasRawDataParameter() == true) {
					CHECK(layoutInfo.pushConstantRangeCount == 0, "Pipeline doen't have two or more RawDataParameter.");

					pushConstantRange.stageFlags = VK_SHADER_STAGE_ALL_GRAPHICS; // Different in ComputePipelineStateVk
					pushConstantRange.offset = 0;
					pushConstantRange.size = layout->GetRawDataParameterSize();

					layoutInfo.pushConstantRangeCount = 1;
					layoutInfo.pPushConstantRanges = &pushConstantRange;
				}

				descSetLayouts[i] = layout->GetVkDescriptorSetLayout();
			}

			layoutInfo.setLayoutCount = SCast(Uint32)(descSetLayouts.size());
			layoutInfo.pSetLayouts = descSetLayouts.data();

			mLayout = device.GetLogicalDevice()->CreateVkPipelineLayoutWrapper(layoutInfo,
				fmt::format("PipelineLayout for '{0}'", attr.debugName).c_str());

			// Graphics pipeline
			VkGraphicsPipelineCreateInfo pipelineCreateInfo = {};
			pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
			pipelineCreateInfo.pNext = nullptr;
			pipelineCreateInfo.flags = 0;
			pipelineCreateInfo.layout = mLayout.mObject;
			pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE; // TODO: 구현?
			pipelineCreateInfo.basePipelineIndex = 0;
			pipelineCreateInfo.pVertexInputState = &vertexInputCreateInfo;
			pipelineCreateInfo.pInputAssemblyState = &inputAssemblyStateCreateInfo;
			pipelineCreateInfo.pRasterizationState = &rasterizationStateCreateInfo;
			pipelineCreateInfo.pColorBlendState = &colorBlendStateCreateInfo;
			pipelineCreateInfo.pTessellationState = nullptr; // TODO: Tessellation 구현시 구현
			pipelineCreateInfo.pMultisampleState = &multisampleStateCreateInfo;
			pipelineCreateInfo.pDynamicState = &dynamicStateCreateInfo;
			pipelineCreateInfo.pViewportState = &viewportStateCreateInfo;
			pipelineCreateInfo.pDepthStencilState = &depthStencilStateCreateInfo;
			pipelineCreateInfo.stageCount = SCast(Uint32)(shaderStages.size());
			pipelineCreateInfo.pStages = shaderStages.data();
			pipelineCreateInfo.renderPass = DPCast(RenderPassVk)(attr.renderPass)->GetHandle();
			pipelineCreateInfo.subpass = 0;

			mPipeline = device.GetLogicalDevice()->CreateVkPipelineWrapper(pipelineCreateInfo, VK_NULL_HANDLE, attr.debugName);
		}

		GraphicsPipelineStateVk::~GraphicsPipelineStateVk()
		{
			mLayout.Release();
			mPipeline.Release();
		}
	} // namespace render
} // namespace cube
