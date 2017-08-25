#pragma once

#include "..\VulkanAPIHeader.h"

#include "BaseRenderAPI\Wrapper\BaseRenderGraphicsPipeline.h"

namespace cube
{
	namespace core
	{
		VkPipelineStageFlags GetVkPipelineStageFlags(PipelineStageBits stageBits);

		class VULKAN_API_EXPORT VulkanGraphicsPipeline : public BaseRenderGraphicsPipeline
		{
		public:
			VulkanGraphicsPipeline(const SPtr<VulkanDevice> device);
			virtual ~VulkanGraphicsPipeline();

			operator VkPipeline() const
			{
				return mPipeline;
			}

			// TODO: binding으로 여러 vertex정보를 추가...? (지금은 0번만 씀)
			void AddVertexInputAttribute(uint32_t location, DataFormat format, uint32_t offset) override;
			void SetVertexInput(uint32_t sizePerVertex) override;

			void SetVertexTopology(VertexTopology topology) override;

			void AddViewport(Viewport viewport, bool isDynamic) override;
			void AddScissor(Rect2D scissor, bool isDynamic) override;
			void SetViewportState() override;

			void SetRasterizer(PolygonMode polygonMode, PolygonFrontFace polygonFrontFace, CullMode cullMode) override;

			void SetMultisampler() override;

			void SetDepthStencil(bool depthTestEnable, bool depthBoundsTestEnable, bool depthWriteEnable, CompareOperator depthCompareOperator,
				bool stencilTestEnable, StencilOperatorState front, StencilOperatorState back, float minDepthBounds, float maxDepthBounds) override;

			void AddColorBlendAttachment(bool blendEnable,
				BlendFactor srcColorBlendFactor, BlendFactor dstColorBlendFactor, BlendOperator colorBlendOp,
				BlendFactor srcAlphaBlendFactor, BlendFactor dstAlphaBlendFactor, BlendOperator alphaBlendOp) override;
			void SetColorBlend(bool logicOpEnable, LogicOperator logicOp,
				float blendConstant1, float blendConstant2, float blendConstant3, float blendConstant4) override;

			void AddShader(SPtr<BaseRenderShader>& shader) override;

			void AddDescriptorSet(SPtr<BaseRenderDescriptorSet>& descSet) override;

			void Create(SPtr<BaseRenderRenderPass>& renderPass) override;

			// ----------------

			/*void SetMultisampleState(VkSampleCountFlagBits rasterizationSamples, VkBool32 sampleShadingEnable,
				float minSampleShading, const VkSampleMask* pSampleMask, VkBool32 alphaToConverageEnable, VkBool32 alphaToOneEnable);*/

			const VkPipelineLayout GetLayout() const;

		private:
			void AddDynamicState(VkDynamicState state);

			VkPipeline mPipeline;
			VkPipelineLayout mPipelineLayout;

			VkPipelineVertexInputStateCreateInfo mVertexInputStateCreateInfo;
			Vector<VkVertexInputBindingDescription> mVertexInputBindingDescs;
			Vector<VkVertexInputAttributeDescription> mVertexInputAttributeDescs;

			VkPipelineInputAssemblyStateCreateInfo mInputAssemblyStateCreateInfo;

			VkPipelineViewportStateCreateInfo mViewportStateCreateInfo;
			Vector<VkViewport> mViewports;
			Vector<VkRect2D> mScissors;

			VkPipelineRasterizationStateCreateInfo mRasterizationStateCreateInfo;

			VkPipelineMultisampleStateCreateInfo mMultisampleStateCreateInfo;

			VkPipelineDepthStencilStateCreateInfo mDepthStencilStateCreateInfo;

			VkPipelineColorBlendStateCreateInfo mColorBlendStateCreateInfo;
			Vector<VkPipelineColorBlendAttachmentState> mColorBlendAttachments;

			Vector<VkDynamicState> mDynamicStates;
			VkPipelineDynamicStateCreateInfo mDynamicStateCreateInfo;

			Vector<VkPipelineShaderStageCreateInfo> mShaderStages;

			Vector<SPtr<VulkanDescriptorSet>> mDescriptorSets;

			SPtr<VulkanDevice> mDevice_ref;
		};

		inline const VkPipelineLayout VulkanGraphicsPipeline::GetLayout() const
		{
			return mPipelineLayout;
		}
	}
}