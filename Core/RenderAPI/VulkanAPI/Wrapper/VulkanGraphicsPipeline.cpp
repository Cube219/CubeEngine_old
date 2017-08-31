#include "VulkanGraphicsPipeline.h"

#include "VulkanDevice.h"
#include "VulkanRenderPass.h"
#include "VulkanShader.h"
#include "VulkanDescriptor.h"

namespace cube
{
	namespace core
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

		VulkanGraphicsPipeline::VulkanGraphicsPipeline(const SPtr<VulkanDevice> device) :
			mDevice_ref(device)
		{
		}

		VulkanGraphicsPipeline::~VulkanGraphicsPipeline()
		{
			vkDestroyPipelineLayout(*mDevice_ref, mPipelineLayout, nullptr);
			vkDestroyPipeline(*mDevice_ref, mPipeline, nullptr);

			mVertexInputAttributeDescs.clear();
			mViewports.clear();
			mScissors.clear();
			mDynamicStates.clear();
			mShaderStages.clear();
		}

		void VulkanGraphicsPipeline::AddVertexInputAttribute(uint32_t location, DataFormat format, uint32_t offset)
		{
			VkVertexInputAttributeDescription attributeDesc;
			attributeDesc.location = location;
			attributeDesc.binding = 0;
			attributeDesc.format = GetVkFormat(format);
			attributeDesc.offset = offset;

			mVertexInputAttributeDescs.push_back(attributeDesc);
		}

		void VulkanGraphicsPipeline::SetVertexInput(uint32_t sizePerVertex)
		{
			// TODO: 일단 binding을 0만 설정 (차후 확장할수도?)
			mVertexInputBindingDesc.binding = 0;
			mVertexInputBindingDesc.stride = sizePerVertex;
			mVertexInputBindingDesc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

			mVertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
			mVertexInputStateCreateInfo.pNext = nullptr;
			mVertexInputStateCreateInfo.flags = 0;
			mVertexInputStateCreateInfo.vertexBindingDescriptionCount = 1;
			mVertexInputStateCreateInfo.pVertexBindingDescriptions = &mVertexInputBindingDesc;
			mVertexInputStateCreateInfo.vertexAttributeDescriptionCount = SCast(uint32_t)(mVertexInputAttributeDescs.size());
			mVertexInputStateCreateInfo.pVertexAttributeDescriptions = mVertexInputAttributeDescs.data();
		}

		void VulkanGraphicsPipeline::SetVertexTopology(VertexTopology topology)
		{
			mInputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
			mInputAssemblyStateCreateInfo.pNext = nullptr;
			mInputAssemblyStateCreateInfo.flags = 0;
			mInputAssemblyStateCreateInfo.topology = GetVkPrimitiveTopology(topology);
			mInputAssemblyStateCreateInfo.primitiveRestartEnable = VK_FALSE;
		}

		void VulkanGraphicsPipeline::AddViewport(Viewport viewport, bool isDynamic)
		{
			if(isDynamic == true) {
				AddDynamicState(VK_DYNAMIC_STATE_VIEWPORT);
			} else {
				mViewports.push_back(GetVkViewport(viewport));
			}
		}

		void VulkanGraphicsPipeline::AddScissor(Rect2D scissor, bool isDynamic)
		{
			if(isDynamic == true) {
				AddDynamicState(VK_DYNAMIC_STATE_SCISSOR);
			} else {
				mScissors.push_back(GetVkRect2D(scissor));
			}
		}

		void VulkanGraphicsPipeline::SetViewportState()
		{
			mViewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
			mViewportStateCreateInfo.pNext = nullptr;
			mViewportStateCreateInfo.flags = 0;
			mViewportStateCreateInfo.viewportCount = SCast(uint32_t)(mViewports.size());
			mViewportStateCreateInfo.pViewports = mViewports.data();
			mViewportStateCreateInfo.scissorCount = SCast(uint32_t)(mScissors.size());
			mViewportStateCreateInfo.pScissors = mScissors.data();
		}

		void VulkanGraphicsPipeline::SetRasterizer(PolygonMode polygonMode, PolygonFrontFace polygonFrontFace, CullMode cullMode)
		{
			mRasterizationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
			mRasterizationStateCreateInfo.pNext = nullptr;
			mRasterizationStateCreateInfo.flags = 0;
			mRasterizationStateCreateInfo.polygonMode = GetVkPolygonMode(polygonMode);
			mRasterizationStateCreateInfo.cullMode = GetVkCullModeFlags(cullMode);
			mRasterizationStateCreateInfo.frontFace = GetVkFrontFace(polygonFrontFace);
			mRasterizationStateCreateInfo.depthClampEnable = VK_TRUE;
			mRasterizationStateCreateInfo.rasterizerDiscardEnable = VK_FALSE;
			mRasterizationStateCreateInfo.depthBiasEnable = VK_FALSE;
			mRasterizationStateCreateInfo.depthBiasConstantFactor = 0;
			mRasterizationStateCreateInfo.depthBiasClamp = 0;
			mRasterizationStateCreateInfo.depthBiasSlopeFactor = 0;
			mRasterizationStateCreateInfo.lineWidth = 1.0f;
		}

		void VulkanGraphicsPipeline::SetMultisampler()
		{
			mMultisampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
			mMultisampleStateCreateInfo.pNext = nullptr;
			mMultisampleStateCreateInfo.flags = 0;
			mMultisampleStateCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
			mMultisampleStateCreateInfo.sampleShadingEnable = VK_FALSE;
			mMultisampleStateCreateInfo.minSampleShading = 0.0f;
			mMultisampleStateCreateInfo.pSampleMask = nullptr;
			mMultisampleStateCreateInfo.alphaToCoverageEnable = VK_FALSE;
			mMultisampleStateCreateInfo.alphaToOneEnable = VK_FALSE;
		}

		void VulkanGraphicsPipeline::SetDepthStencil(bool depthTestEnable, bool depthBoundsTestEnable, bool depthWriteEnable, CompareOperator depthCompareOperator,
			bool stencilTestEnable, StencilOperatorState front, StencilOperatorState back, float minDepthBounds, float maxDepthBounds)
		{
			mDepthStencilStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
			mDepthStencilStateCreateInfo.pNext = nullptr;
			mDepthStencilStateCreateInfo.flags = 0;
			mDepthStencilStateCreateInfo.depthTestEnable = depthTestEnable;
			mDepthStencilStateCreateInfo.depthWriteEnable = depthWriteEnable;
			mDepthStencilStateCreateInfo.depthCompareOp = GetVkCompareOp(depthCompareOperator);
			mDepthStencilStateCreateInfo.depthBoundsTestEnable = depthBoundsTestEnable;
			mDepthStencilStateCreateInfo.stencilTestEnable = stencilTestEnable;
			mDepthStencilStateCreateInfo.back = GetVkStencilOpState(back);
			mDepthStencilStateCreateInfo.front = GetVkStencilOpState(front);
			mDepthStencilStateCreateInfo.minDepthBounds = minDepthBounds;
			mDepthStencilStateCreateInfo.maxDepthBounds = maxDepthBounds;
		}

		void VulkanGraphicsPipeline::AddColorBlendAttachment(bool blendEnable,
			BlendFactor srcColorBlendFactor, BlendFactor dstColorBlendFactor, BlendOperator colorBlendOp,
			BlendFactor srcAlphaBlendFactor, BlendFactor dstAlphaBlendFactor, BlendOperator alphaBlendOp)
		{
			VkPipelineColorBlendAttachmentState attachment;
			attachment.blendEnable = blendEnable;
			attachment.srcColorBlendFactor = GetVkBlendFactor(srcColorBlendFactor);
			attachment.dstColorBlendFactor = GetVkBlendFactor(dstColorBlendFactor);
			attachment.colorBlendOp = GetVkBlendOp(colorBlendOp);
			attachment.srcAlphaBlendFactor = GetVkBlendFactor(srcAlphaBlendFactor);
			attachment.dstAlphaBlendFactor = GetVkBlendFactor(dstAlphaBlendFactor);
			attachment.alphaBlendOp = GetVkBlendOp(alphaBlendOp);
			attachment.colorWriteMask = 0xf; // TODO: 일단 RGBA 전체

			mColorBlendAttachments.push_back(attachment);
		}

		void VulkanGraphicsPipeline::SetColorBlend(bool logicOpEnable, LogicOperator logicOp,
			float blendConstant1, float blendConstant2, float blendConstant3, float blendConstant4)
		{
			mColorBlendStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
			mColorBlendStateCreateInfo.pNext = nullptr;
			mColorBlendStateCreateInfo.flags = 0;
			mColorBlendStateCreateInfo.logicOpEnable = logicOpEnable;
			mColorBlendStateCreateInfo.logicOp = GetVkLogicOp(logicOp);
			mColorBlendStateCreateInfo.attachmentCount = SCast(uint32_t)(mColorBlendAttachments.size());
			mColorBlendStateCreateInfo.pAttachments = mColorBlendAttachments.data();
			mColorBlendStateCreateInfo.blendConstants[0] = blendConstant1;
			mColorBlendStateCreateInfo.blendConstants[1] = blendConstant2;
			mColorBlendStateCreateInfo.blendConstants[2] = blendConstant3;
			mColorBlendStateCreateInfo.blendConstants[3] = blendConstant4;
		}

		void VulkanGraphicsPipeline::AddDynamicState(VkDynamicState state)
		{
			if(mDynamicStates.size() >= VK_DYNAMIC_STATE_RANGE_SIZE) {
				PrintLogWithSayer(L"VulkanPipeline", L"Cannot add more dynamic state");
				return;
			}

			mDynamicStates.push_back(state);
		}

		void VulkanGraphicsPipeline::AddShader(SPtr<BaseRenderShader>& shader)
		{
			mShaderStages.push_back(SPCast(VulkanShader)(shader)->GetStageInfo());
		}

		void VulkanGraphicsPipeline::AddDescriptorSet(SPtr<BaseRenderDescriptorSet>& descSet)
		{
			mDescriptorSets.push_back(SPCast(VulkanDescriptorSet)(descSet));
		}

		void VulkanGraphicsPipeline::Create(SPtr<BaseRenderRenderPass>& renderPass)
		{
			VkResult res;

			// Set dynamic state
			mDynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
			mDynamicStateCreateInfo.pNext = nullptr;
			mDynamicStateCreateInfo.flags = 0;
			mDynamicStateCreateInfo.dynamicStateCount = SCast(uint32_t)(mDynamicStates.size());
			mDynamicStateCreateInfo.pDynamicStates = mDynamicStates.data();

			// Create pipeline layout
			VkDescriptorSetLayout* layouts = new VkDescriptorSetLayout[mDescriptorSets.size()];
			for(int i = 0; i < mDescriptorSets.size(); i++) {
				layouts[i] = mDescriptorSets[i]->GetLayout();
			}

			VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};
			pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
			pipelineLayoutCreateInfo.pNext = nullptr;
			pipelineLayoutCreateInfo.flags = 0;
			pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
			pipelineLayoutCreateInfo.pPushConstantRanges = nullptr;
			pipelineLayoutCreateInfo.setLayoutCount = SCast(uint32_t)(mDescriptorSets.size());
			pipelineLayoutCreateInfo.pSetLayouts = layouts;

			res = vkCreatePipelineLayout(*mDevice_ref, &pipelineLayoutCreateInfo, nullptr, &mPipelineLayout);
			CheckVkResult(L"VulkanPipeline", L"Cannot create a pipeline layout", res);

			delete[] layouts;

			// Creat pipeline
			VkGraphicsPipelineCreateInfo pipelineCreateInfo = {};
			pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
			pipelineCreateInfo.pNext = nullptr;
			pipelineCreateInfo.flags = 0;
			pipelineCreateInfo.layout = mPipelineLayout;
			pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
			pipelineCreateInfo.basePipelineIndex = 0;
			pipelineCreateInfo.pVertexInputState = &mVertexInputStateCreateInfo;
			pipelineCreateInfo.pInputAssemblyState = &mInputAssemblyStateCreateInfo;
			pipelineCreateInfo.pRasterizationState = &mRasterizationStateCreateInfo;
			pipelineCreateInfo.pColorBlendState = &mColorBlendStateCreateInfo;
			pipelineCreateInfo.pTessellationState = nullptr;
			pipelineCreateInfo.pMultisampleState = &mMultisampleStateCreateInfo;
			pipelineCreateInfo.pDynamicState = &mDynamicStateCreateInfo;
			pipelineCreateInfo.pViewportState = &mViewportStateCreateInfo;
			pipelineCreateInfo.pDepthStencilState = &mDepthStencilStateCreateInfo;
			pipelineCreateInfo.stageCount = SCast(uint32_t)(mShaderStages.size());
			pipelineCreateInfo.pStages = mShaderStages.data();
			pipelineCreateInfo.renderPass = *DPCast(VulkanRenderPass)(renderPass);
			pipelineCreateInfo.subpass = 0;

			res = vkCreateGraphicsPipelines(*mDevice_ref, nullptr, 1, &pipelineCreateInfo, nullptr, &mPipeline);
			CheckVkResult(L"VulkanPipeline", L"Cannot create a VulkanPipeline", res);
		}
	}
}