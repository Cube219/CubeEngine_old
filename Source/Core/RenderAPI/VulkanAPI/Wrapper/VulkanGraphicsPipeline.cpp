#include "VulkanGraphicsPipeline.h"

#include "VulkanDevice.h"
#include "VulkanRenderPass.h"
#include "VulkanShader.h"
#include "VulkanDescriptor.h"

namespace cube
{
	namespace render
	{
		VkPipelineStageFlags GetVkPipelineStageFlags(PipelineStageBits stageBits)
		{
			VkPipelineStageFlags f = 0;

			if(static_cast<int>(stageBits & PipelineStageBits::TopBit) > 0)
				f |= VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;

			if(static_cast<int>(stageBits & PipelineStageBits::DrawIndirectBit) > 0)
				f |= VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT;

			if(static_cast<int>(stageBits & PipelineStageBits::VertexInputBit) > 0)
				f |= VK_PIPELINE_STAGE_VERTEX_INPUT_BIT;

			if(static_cast<int>(stageBits & PipelineStageBits::VertexShaderBit) > 0)
				f |= VK_PIPELINE_STAGE_VERTEX_SHADER_BIT;

			if(static_cast<int>(stageBits & PipelineStageBits::TessellationControlShaderBit) > 0)
				f |= VK_PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT;

			if(static_cast<int>(stageBits & PipelineStageBits::TessellationEvaluationShaderBit) > 0)
				f |= VK_PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT;

			if(static_cast<int>(stageBits & PipelineStageBits::GeometryShaderBit) > 0)
				f |= VK_PIPELINE_STAGE_GEOMETRY_SHADER_BIT;

			if(static_cast<int>(stageBits & PipelineStageBits::FragmentShaderBit) > 0)
				f |= VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;

			if(static_cast<int>(stageBits & PipelineStageBits::EarlyFragmentShaderBit) > 0)
				f |= VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;

			if(static_cast<int>(stageBits & PipelineStageBits::LateFragmentShaderBit) > 0)
				f |= VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;

			if(static_cast<int>(stageBits & PipelineStageBits::ColorAttachmentOutputBit) > 0)
				f |= VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

			if(static_cast<int>(stageBits & PipelineStageBits::ComputeShaderBit) > 0)
				f |= VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;

			if(static_cast<int>(stageBits & PipelineStageBits::TransferBit) > 0)
				f |= VK_PIPELINE_STAGE_TRANSFER_BIT;

			if(static_cast<int>(stageBits & PipelineStageBits::BottomBit) > 0)
				f |= VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;

			if(static_cast<int>(stageBits & PipelineStageBits::HostBit) > 0)
				f |= VK_PIPELINE_STAGE_HOST_BIT;

			if(static_cast<int>(stageBits & PipelineStageBits::AllGraphicsBit) > 0)
				f |= VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT;

			if(static_cast<int>(stageBits & PipelineStageBits::AllCommandsBit) > 0)
				f |= VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;

			return f;
		}

		VulkanGraphicsPipeline::VulkanGraphicsPipeline(const SPtr<VulkanDevice> device, GraphicsPipelineInitializer& initializer) :
			mDevice_ref(device)
		{
			VkResult res;

			// Set dynamic states
			Vector<VkDynamicState> dynamicState;
			if(initializer.isViewportDynamic == true)
				dynamicState.push_back(VK_DYNAMIC_STATE_VIEWPORT);
			if(initializer.isScissorDynamic == true)
				dynamicState.push_back(VK_DYNAMIC_STATE_SCISSOR);

			VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo;
			dynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
			dynamicStateCreateInfo.pNext = nullptr;
			dynamicStateCreateInfo.flags = 0;
			dynamicStateCreateInfo.dynamicStateCount = SCast(uint32_t)(dynamicState.size());
			dynamicStateCreateInfo.pDynamicStates = dynamicState.data();

			// Create pipeline layout
			Vector<VkDescriptorSetLayout> layouts;
			layouts.resize(initializer.descSetLayouts.size());
			for(int i = 0; i < initializer.descSetLayouts.size(); i++) {
				layouts[i] = SPCast(VulkanDescriptorSetLayout)(initializer.descSetLayouts[i])->GetHandle();
			}

			VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};
			pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
			pipelineLayoutCreateInfo.pNext = nullptr;
			pipelineLayoutCreateInfo.flags = 0;
			pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
			pipelineLayoutCreateInfo.pPushConstantRanges = nullptr;
			pipelineLayoutCreateInfo.setLayoutCount = SCast(uint32_t)(layouts.size());
			pipelineLayoutCreateInfo.pSetLayouts = layouts.data();

			res = vkCreatePipelineLayout(mDevice_ref->GetHandle(), &pipelineLayoutCreateInfo, nullptr, &mPipelineLayout);
			CheckVkResult("Cannot create a pipeline layout", res);

			// Vertex input
			Vector<VkVertexInputAttributeDescription> vertexInputAttributes;
			vertexInputAttributes.resize(initializer.vertexInputAttributes.size());
			for(uint64_t i = 0; i < initializer.vertexInputAttributes.size(); i++) {
				auto attr = initializer.vertexInputAttributes[i];

				vertexInputAttributes[i].location = attr.location;
				vertexInputAttributes[i].binding = 0;
				vertexInputAttributes[i].format = GetVkFormat(attr.format);
				vertexInputAttributes[i].offset = attr.offset;
			}

			VkVertexInputBindingDescription bindingDesc;
			bindingDesc.binding = 0;
			bindingDesc.stride = initializer.vertexSize;
			bindingDesc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

			VkPipelineVertexInputStateCreateInfo vertexInputCreateInfo;
			vertexInputCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
			vertexInputCreateInfo.pNext = nullptr;
			vertexInputCreateInfo.flags = 0;
			vertexInputCreateInfo.vertexBindingDescriptionCount = 1;
			vertexInputCreateInfo.pVertexBindingDescriptions = &bindingDesc;
			vertexInputCreateInfo.vertexAttributeDescriptionCount = SCast(uint32_t)(vertexInputAttributes.size());
			vertexInputCreateInfo.pVertexAttributeDescriptions = vertexInputAttributes.data();

			// Vertex topology
			VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo;
			inputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
			inputAssemblyStateCreateInfo.pNext = nullptr;
			inputAssemblyStateCreateInfo.flags = 0;
			inputAssemblyStateCreateInfo.topology = GetVkPrimitiveTopology(initializer.vertexTopology);
			inputAssemblyStateCreateInfo.primitiveRestartEnable = VK_FALSE;

			// Viewport / Scissor
			VkViewport viewport = {};
			if(initializer.isViewportDynamic == false) {
				viewport = GetVkViewport(initializer.viewport);
			}
			
			VkRect2D scissor = {};
			if(initializer.isScissorDynamic == false) {
				scissor = GetVkRect2D(initializer.scissor);
			}

			VkPipelineViewportStateCreateInfo viewportStateCreateInfo;
			viewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
			viewportStateCreateInfo.pNext = nullptr;
			viewportStateCreateInfo.flags = 0;
			viewportStateCreateInfo.viewportCount = 1;
			viewportStateCreateInfo.pViewports = &viewport;
			viewportStateCreateInfo.scissorCount = 1;
			viewportStateCreateInfo.pScissors = &scissor;

			// Rasterizer
			VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo;
			rasterizationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
			rasterizationStateCreateInfo.pNext = nullptr;
			rasterizationStateCreateInfo.flags = 0;
			rasterizationStateCreateInfo.polygonMode = GetVkPolygonMode(initializer.rasterizer.polygonMode);
			rasterizationStateCreateInfo.cullMode = GetVkCullModeFlags(initializer.rasterizer.cullMode);
			rasterizationStateCreateInfo.frontFace = GetVkFrontFace(initializer.rasterizer.polygonFrontFace);
			rasterizationStateCreateInfo.depthClampEnable = VK_FALSE;
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

			// Depth/Stencil
			VkPipelineDepthStencilStateCreateInfo depthStencilStateCreateInfo;
			depthStencilStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
			depthStencilStateCreateInfo.pNext = nullptr;
			depthStencilStateCreateInfo.flags = 0;
			depthStencilStateCreateInfo.depthTestEnable = initializer.depthStencil.depthTestEnable;
			depthStencilStateCreateInfo.depthWriteEnable = initializer.depthStencil.depthWriteEnable;
			depthStencilStateCreateInfo.depthCompareOp = GetVkCompareOp(initializer.depthStencil.depthCompareOperator);
			depthStencilStateCreateInfo.depthBoundsTestEnable = initializer.depthStencil.depthBoundsTestEnable;
			depthStencilStateCreateInfo.stencilTestEnable = initializer.depthStencil.stencilTestEnable;
			depthStencilStateCreateInfo.back = GetVkStencilOpState(initializer.depthStencil.back);
			depthStencilStateCreateInfo.front = GetVkStencilOpState(initializer.depthStencil.front);
			depthStencilStateCreateInfo.minDepthBounds = initializer.depthStencil.minDepthBounds;
			depthStencilStateCreateInfo.maxDepthBounds = initializer.depthStencil.maxDepthBounds;

			// Color blend
			Vector<VkPipelineColorBlendAttachmentState> colorBlendAttachments;
			colorBlendAttachments.resize(initializer.colorBlendAttachments.size());
			for(uint64_t i = 0; i < initializer.colorBlendAttachments.size(); i++) {
				auto attachment = initializer.colorBlendAttachments[i];

				colorBlendAttachments[i].blendEnable = attachment.blendEnable;
				colorBlendAttachments[i].srcColorBlendFactor = GetVkBlendFactor(attachment.srcColorBlendFactor);
				colorBlendAttachments[i].dstColorBlendFactor = GetVkBlendFactor(attachment.dstColorBlendFactor);
				colorBlendAttachments[i].colorBlendOp = GetVkBlendOp(attachment.colorBlendOp);
				colorBlendAttachments[i].srcAlphaBlendFactor = GetVkBlendFactor(attachment.srcAlphaBlendFactor);
				colorBlendAttachments[i].dstAlphaBlendFactor = GetVkBlendFactor(attachment.dstAlphaBlendFactor);
				colorBlendAttachments[i].alphaBlendOp = GetVkBlendOp(attachment.alphaBlendOp);
				colorBlendAttachments[i].colorWriteMask = 0xf; // TODO: 일단 RGBA 전체
			}

			VkPipelineColorBlendStateCreateInfo colorBlendStateCreateInfo;
			colorBlendStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
			colorBlendStateCreateInfo.pNext = nullptr;
			colorBlendStateCreateInfo.flags = 0;
			colorBlendStateCreateInfo.logicOpEnable = false;
			colorBlendStateCreateInfo.logicOp = VK_LOGIC_OP_NO_OP;
			colorBlendStateCreateInfo.attachmentCount = SCast(uint32_t)(colorBlendAttachments.size());
			colorBlendStateCreateInfo.pAttachments = colorBlendAttachments.data();
			colorBlendStateCreateInfo.blendConstants[0] = 0.0f;
			colorBlendStateCreateInfo.blendConstants[1] = 0.0f;
			colorBlendStateCreateInfo.blendConstants[2] = 0.0f;
			colorBlendStateCreateInfo.blendConstants[3] = 0.0f;

			// Shader
			Vector<VkPipelineShaderStageCreateInfo> shaderStages;
			for(uint64_t i = 0; i < initializer.shaders.size(); i++) {
				shaderStages.push_back(DPCast(VulkanShader)(initializer.shaders[i])->GetStageInfo());
			}

			// Creat pipeline
			VkGraphicsPipelineCreateInfo pipelineCreateInfo = {};
			pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
			pipelineCreateInfo.pNext = nullptr;
			pipelineCreateInfo.flags = 0;
			pipelineCreateInfo.layout = mPipelineLayout;
			pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
			pipelineCreateInfo.basePipelineIndex = 0;
			pipelineCreateInfo.pVertexInputState = &vertexInputCreateInfo;
			pipelineCreateInfo.pInputAssemblyState = &inputAssemblyStateCreateInfo;
			pipelineCreateInfo.pRasterizationState = &rasterizationStateCreateInfo;
			pipelineCreateInfo.pColorBlendState = &colorBlendStateCreateInfo;
			pipelineCreateInfo.pTessellationState = nullptr;
			pipelineCreateInfo.pMultisampleState = &multisampleStateCreateInfo;
			pipelineCreateInfo.pDynamicState = &dynamicStateCreateInfo;
			pipelineCreateInfo.pViewportState = &viewportStateCreateInfo;
			pipelineCreateInfo.pDepthStencilState = &depthStencilStateCreateInfo;
			pipelineCreateInfo.stageCount = SCast(uint32_t)(shaderStages.size());
			pipelineCreateInfo.pStages = shaderStages.data();
			pipelineCreateInfo.renderPass = DPCast(VulkanRenderPass)(initializer.renderPass)->GetHandle();
			pipelineCreateInfo.subpass = 0;

			res = vkCreateGraphicsPipelines(mDevice_ref->GetHandle(), nullptr, 1, &pipelineCreateInfo, nullptr, &mPipeline);
			CheckVkResult("Cannot create a VulkanPipeline", res);
		}

		VulkanGraphicsPipeline::~VulkanGraphicsPipeline()
		{
			vkDestroyPipelineLayout(mDevice_ref->GetHandle(), mPipelineLayout, nullptr);
			vkDestroyPipeline(mDevice_ref->GetHandle(), mPipeline, nullptr);
		}
	} // namespace render
} // namespace cube
